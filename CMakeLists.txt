cmake_minimum_required(VERSION 3.16)
project(project VERSION 1.0 LANGUAGES CXX C)
# Set policy to silence Warnings about timestamps of downloaded files.
# Review occasionally if it may be set to NEW
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()

SET(LJMD_VERSION "1.0" STRING)

add_executable(MAIN.x src/main.cpp)
target_include_directories(MAIN.x PUBLIC inc)

add_library(Timer inc/Timer.hpp)
set_target_properties(Timer PROPERTIES LINKER_LANGUAGE CXX)

add_library(mdlib src/comp.c src/verlet.c src/output.c src/utils.c src/init.c
                  src/structs.c src/cleanup.c)
target_include_directories(mdlib PUBLIC inc)
target_link_libraries(mdlib Timer)

target_link_libraries(MAIN.x mdlib)

option(MY_MPI "Enable MPI calls" OFF)
IF(MY_MPI)
  add_library(myMPI inc/myMPI.hpp)
  set_target_properties(myMPI PROPERTIES LINKER_LANGUAGE CXX)
  FIND_PACKAGE(MPI REQUIRED)
  IF(MPI_FOUND)
    target_link_libraries(myMPI MPI::MPI_CXX)
  ENDIF()
  target_link_libraries(MAIN.x myMPI)
ENDIF()

option(LJMD_OPENMP "Enable OpenMP calls" OFF)
IF(LJMD_OPENMP)
  FIND_PACKAGE(OpenMP REQUIRED)
  IF(OpenMP_FOUND)
    target_link_libraries(mdlib OpenMP::OpenMP_C)
    target_link_libraries(mdlib OpenMP::OpenMP_CXX)
  ENDIF()
ENDIF()

option(TEST "Enable Unit Tests" OFF)
if(TEST)
  enable_testing()
  enable_language(CXX)

  set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
  include(GTest)
  add_executable(test_verlet test/test_verlet.cpp)
  target_include_directories(test_verlet PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_link_libraries(test_verlet PRIVATE GTest::GTestMain GTest::GTest mdlib)
  add_test(NAME Verlet COMMAND test_verlet)

  add_executable(test_utils test/test_utils.cpp)
  target_include_directories(test_utils PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_link_libraries(test_utils PRIVATE GTest::GTestMain GTest::GTest mdlib)
  add_test(NAME Utils COMMAND test_utils)

  add_executable(test_comp test/test_comp.cpp)
  target_include_directories(test_comp PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_link_libraries(test_comp PRIVATE GTest::GTestMain GTest::GTest mdlib)
  add_test(NAME Comp COMMAND test_comp)
endif()
