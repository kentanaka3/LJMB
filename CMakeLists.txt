cmake_minimum_required(VERSION 3.16)
project(project VERSION 1.0 LANGUAGES CXX C)
# Set policy to silence Warnings about timestamps of downloaded files.
# Review occasionally if it may be set to NEW
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 OLD)
endif()


add_executable(MAIN.x src/main.cpp)

add_library(Timer inc/Timer.hpp)
set_target_properties(Timer PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(MAIN.x Timer)





add_library(mdlib util/utils.c src/verlet.c)






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
