#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <map>

using time_units = std::chrono::milliseconds;

struct TimerData{
  int calls;
  time_units t;
  TimerData(const int& n, const time_units& val): calls(n), t(val){};
  TimerData(){};
};

std::map<std::string, TimerData> timeTable;
void print_timing_results() {
  #ifdef _MPI
  std::cout << std::endl << "- TIMING RESULTS -" << std::endl;
  extern int myPE, nPEs;
  auto sample = timeTable.begin() -> second.t.count();
  std::vector<decltype(sample)> times;
  std::vector<decltype(sample)> allTimes;
  int size = timeTable.size();
  times.resize(size);
  int t_el{0};
  for (const auto& pair : timeTable) {
    times[t_el] = pair.second.t.count();
    t_el++;
  }
  if (!myPE)
    allTimes.resize(nPEs*size, 0);
  MPI_Gather(reinterpret_cast<char*>(times.data()), size*sizeof(times[0]), \
             MPI_CHAR, \
             reinterpret_cast<char*>(allTimes.data()), size*sizeof(times[0]), \
             MPI_CHAR, 0, MPI_COMM_WORLD);
  if (!myPE) {
    t_el = 0;
    for (const auto& pair : timeTable) {
      std::cout << pair.first << ": "<< pair.second.calls << std::endl;
      for (int j = 0; j < nPEs; j++) {
        std::cout<< allTimes[j*size + t_el] << " ";
      }
      std::cout << std::endl;
      t_el++;
    }
  }
  #else
  std::cout << std::endl << "- TIMING RESULTS -" << std::endl;
  for (const auto & pair : timeTable) {
    std::cout << pair.first << ": " << pair.second.calls << std::endl;
    std::cout << pair.second.t.count() << std::endl;
  }
  #endif
};

class CSimpleTimer {
public:
  std::string name;
  std::chrono::time_point<std::chrono::steady_clock> start;
  CSimpleTimer(const std::string& s): name(s) {
    if (timeTable.find(name) != timeTable.end())
      timeTable[name].calls++;
    else
      timeTable.insert(std::make_pair(name, TimerData(1, time_units(0))));
    start = std::chrono::steady_clock::now();
  };
  ~CSimpleTimer() {
    auto duration = std::chrono::duration_cast<time_units>(
                      std::chrono::steady_clock::now() - start);
    timeTable[name].t += duration;
  };
};
#ifdef MY_CU
class CUSimpleTimer {
public:
  std::string name;
  cudaEvent_t start, stop;
  CUSimpleTimer(const std::string& s): name(s) {
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    if (timeTable.find(name) != timeTable.end())
      timeTable[name].calls++;
    else
      timeTable.insert(std::make_pair(name, TimerData(1, time_units(0))));
    cudaEventRecord(start);
  };
  ~CUSimpleTimer() {
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float msecs = 0;
    cudaEventElapsedTime(&msecs, start, stop);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    timeTable[name].t += \
      std::chrono::round<time_units>(std::chrono::duration<float>{msecs});
  };
};
#endif