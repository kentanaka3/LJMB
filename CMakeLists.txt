cmake_minimum_required(VERSION 3.16)
project(project VERSION 1.0 LANGUAGES CXX C)
# Set policy to silence Warnings about timestamps of downloaded files.
# Review occasionally if it may be set to NEW
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()

option(LJMD_VERSION "LJMD Version" 1.0)

add_executable(MAIN.x src/main.cpp)
target_include_directories(MAIN.x PUBLIC inc)

add_library(Timer inc/Timer.hpp)
set_target_properties(Timer PROPERTIES LINKER_LANGUAGE CXX)

add_library(init src/init.c)
set_target_properties(init PROPERTIES LINKER_LANGUAGE C)
target_include_directories(init PUBLIC inc)

add_library(structs inc/structs.h)
set_target_properties(structs PROPERTIES LINKER_LANGUAGE C)
target_include_directories(structs PUBLIC inc)

add_library(utils util/utils.c)
set_target_properties(utils PROPERTIES LINKER_LANGUAGE C)
target_include_directories(utils PUBLIC inc)

add_library(output src/output.c)
set_target_properties(output PROPERTIES LINKER_LANGUAGE C)
target_include_directories(output PUBLIC inc)

add_library(comp src/comp.c)
set_target_properties(comp PROPERTIES LINKER_LANGUAGE C)
target_include_directories(comp PUBLIC inc)

add_library(verlet src/verlet.c)
set_target_properties(verlet PROPERTIES LINKER_LANGUAGE C)
target_include_directories(verlet PUBLIC inc)

target_link_libraries(utils structs)
target_link_libraries(output structs)
target_link_libraries(comp structs utils)
target_link_libraries(init structs utils)
target_link_libraries(MAIN.x Timer init structs output comp verlet)


add_library(mdlib util/utils.c src/verlet.c)

option(MY_MPI "Enable MPI calls" OFF)
if(MY_MPI)
  add_library(myMPI inc/myMPI.hpp)
  set_target_properties(myMPI PROPERTIES LINKER_LANGUAGE CXX)
  target_link_libraries(MAIN.x myMPI)
endif()

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
endif()
