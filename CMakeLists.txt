cmake_minimum_required(VERSION 3.20)
project(xgbtest)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
list(APPEND CMAKE_PREFIX_PATH xgboost)

find_package(xgboost REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

link_directories(${PROJECT_SOURCE_DIR}/LightGBM/lib)

add_executable(xgbtest main.cpp XgbTest.cpp LgbTest.cpp)
include_directories(${PROJECT_SOURCE_DIR}/LightGBM/include)
target_link_libraries(xgbtest xgboost::xgboost pthread _lightgbm)
