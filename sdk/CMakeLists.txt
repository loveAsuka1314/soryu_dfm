cmake_minimum_required(VERSION 3.22)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(soryu VERSION 0.1.0 LANGUAGES CXX)
endif()

if(NOT ANDROID OR NOT CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
    message(FATAL_ERROR "Use the Android NDK toolchain with -DANDROID_ABI=arm64-v8a")
endif()
if(ANDROID_NATIVE_API_LEVEL LESS 26)
    message(FATAL_ERROR "Use -DANDROID_PLATFORM=android-26 or newer")
endif()

option(SORYU_BUILD_TOOLS "Build the soryu_loader command-line tool" ON)
option(SORYU_BUILD_EXAMPLE "Build the SDK calling example" ON)

# The kernel ABI is the only source of truth, including in installed SDKs.
configure_file(../abi.h include/dfm/abi.h COPYONLY)

add_library(soryu_sdk STATIC src/driver.cpp src/loader.cpp)
add_library(soryu::sdk ALIAS soryu_sdk)
set_target_properties(soryu_sdk PROPERTIES
    EXPORT_NAME sdk
    POSITION_INDEPENDENT_CODE ON
)
target_compile_features(soryu_sdk PUBLIC cxx_std_20)
target_compile_options(soryu_sdk PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(soryu_sdk PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(soryu_sdk PUBLIC log)
# Propagate ELF alignment to tools and external consumers for 16 KiB devices.
target_link_options(soryu_sdk INTERFACE "LINKER:-z,max-page-size=16384")

if(SORYU_BUILD_TOOLS)
    add_executable(soryu_loader tools/soryu_loader.cpp)
    target_link_libraries(soryu_loader PRIVATE soryu::sdk)
    target_compile_options(soryu_loader PRIVATE -Wall -Wextra -Wpedantic)
    install(TARGETS soryu_loader RUNTIME DESTINATION bin)
endif()
if(SORYU_BUILD_EXAMPLE)
    add_executable(soryu_example examples/basic.cpp)
    target_link_libraries(soryu_example PRIVATE soryu::sdk)
    target_compile_options(soryu_example PRIVATE -Wall -Wextra -Wpedantic)
    install(TARGETS soryu_example RUNTIME DESTINATION bin)
endif()

include(CMakePackageConfigHelpers)
configure_package_config_file(cmake/soryuConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/soryuConfig.cmake"
    INSTALL_DESTINATION lib/cmake/soryu
)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/soryuConfigVersion.cmake"
    VERSION 0.1.0 COMPATIBILITY SameMajorVersion
)
install(TARGETS soryu_sdk EXPORT soryuTargets ARCHIVE DESTINATION lib)
install(EXPORT soryuTargets NAMESPACE soryu:: DESTINATION lib/cmake/soryu)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/soryuConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/soryuConfigVersion.cmake"
    DESTINATION lib/cmake/soryu
)
install(DIRECTORY include/dfm DESTINATION include)
install(FILES ../abi.h DESTINATION include/dfm)
install(FILES examples/basic.cpp examples/CMakeLists.txt DESTINATION examples)
