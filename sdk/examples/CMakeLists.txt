cmake_minimum_required(VERSION 3.22)
project(soryu_sdk_example LANGUAGES CXX)

# Configure with the NDK toolchain and
# -Dsoryu_DIR=/path/to/installed/sdk/lib/cmake/soryu.
find_package(soryu CONFIG REQUIRED)
add_executable(soryu_example basic.cpp)
target_link_libraries(soryu_example PRIVATE soryu::sdk)
