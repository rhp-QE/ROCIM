cmake_minimum_required(VERSION 3.13.0)

# 设置 C++ 标准为 11
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_program(CMAKE_C_COMPILER clang)
find_program(CMAKE_CXX_COMPILER clang++)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS_OUTPUT_PATH "../")
set (CMAKE_TOOLCHAIN_FILE "/root/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")


# -------------------------------------
project(ROCIM_CLIENT VERSION 0.1.0 LANGUAGES C CXX)

# -------------------------------------c

include_directories(${CMAKE_SOURCE_DIR})

# src
file(GLOB_RECURSE SOURCES "im/*.cpp")
file(GLOB_RECURSE SOURCES1 "im/*.cc")

# main
list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/main.cpp)

add_executable(main ${SOURCES} ${SOURCES1})

#--------------------------------------

find_package(folly CONFIG REQUIRED)
target_link_libraries(main PRIVATE Folly::folly Folly::folly_deps Folly::follybenchmark Folly::folly_test_util)

# WCDB
target_include_directories(main PRIVATE "/root/wcdb/src/build/export_headers")
target_link_directories(main PRIVATE "/root/wcdb/src/build/")
target_link_libraries(main PRIVATE WCDB)

#cpp-httplib
find_package(httplib CONFIG REQUIRED)
target_link_libraries(main PRIVATE httplib::httplib)

#boost_json
find_package(Boost REQUIRED COMPONENTS json)
target_link_libraries(main PRIVATE Boost::json)

#protobuf
target_include_directories(main PRIVATE "/root/share/protobuf/include")
target_link_directories(main PRIVATE "/root/share/protobuf/lib")
target_link_libraries(main PRIVATE protoc protobuf protobuf-lite)

# 设置默认构建类型为 Debug（可选）
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 显式添加调试编译选项（通常 CMake 会自动处理）
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")


include(CTest)
enable_testing()

