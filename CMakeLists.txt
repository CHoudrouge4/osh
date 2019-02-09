cmake_minimum_required(VERSION 3.10)

project(labs)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -O3")

add_executable(labs.exe ${SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(labs.exe Threads::Threads)
