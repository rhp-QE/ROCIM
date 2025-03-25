cmake_minimum_required(VERSION 3.28.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
find_program(CMAKE_C_COMPILER clang)
find_program(CMAKE_CXX_COMPILER clang++)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_EXPORT_COMPILE_COMMANDS_OUTPUT_PATH "../")


# -------------------------------------
project(ROCIM_CLIENT VERSION 0.1.0 LANGUAGES C CXX)

# -------------------------------------c

include_directories(${CMAKE_SOURCE_DIR})

# 使用更安全的源文件收集方式（推荐显式列出）
file(GLOB_RECURSE SOURCES 
  "im/*.cpp"
  "im/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

add_executable(main ${SOURCES})

#--------------------------------------

# WCDB
target_include_directories(main PRIVATE "/root/wcdb/src/build/export_headers")
target_link_directories(main PRIVATE "/root/wcdb/src/build/")
target_link_libraries(main PRIVATE WCDB)

# BOOST
# --------------------------------------------
set(Boost_DIR "/root/ccdep/boost_1_84_0/lib/cmake/Boost-1.84.0")
set(BOOST_ROOT "/root/ccdep/boost_1_84_0")

find_package(Boost REQUIRED COMPONENTS asio json system)  # 合并查找

target_link_libraries(main PRIVATE 
    Boost::asio
    Boost::json 
    Boost::system
)
# --------------------------------------------

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

