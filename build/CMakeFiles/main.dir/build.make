# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/ROCIM/ROCIM_CLIENT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/ROCIM/ROCIM_CLIENT/build

# Include any dependencies generated for this target.
include CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.dir/flags.make

CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o: ../src/net/LongConnectionImpl.cpp
CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ROCIM/ROCIM_CLIENT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o -MF CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o.d -o CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o -c /root/ROCIM/ROCIM_CLIENT/src/net/LongConnectionImpl.cpp

CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ROCIM/ROCIM_CLIENT/src/net/LongConnectionImpl.cpp > CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.i

CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ROCIM/ROCIM_CLIENT/src/net/LongConnectionImpl.cpp -o CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.s

CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/main.cpp.o: ../main.cpp
CMakeFiles/main.dir/main.cpp.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ROCIM/ROCIM_CLIENT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/main.dir/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/main.cpp.o -MF CMakeFiles/main.dir/main.cpp.o.d -o CMakeFiles/main.dir/main.cpp.o -c /root/ROCIM/ROCIM_CLIENT/main.cpp

CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ROCIM/ROCIM_CLIENT/main.cpp > CMakeFiles/main.dir/main.cpp.i

CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ROCIM/ROCIM_CLIENT/main.cpp -o CMakeFiles/main.dir/main.cpp.s

CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o: CMakeFiles/main.dir/flags.make
CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o: ../src/base/LinkBuffer.cpp
CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o: CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/ROCIM/ROCIM_CLIENT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o -MF CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o.d -o CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o -c /root/ROCIM/ROCIM_CLIENT/src/base/LinkBuffer.cpp

CMakeFiles/main.dir/src/base/LinkBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/src/base/LinkBuffer.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/ROCIM/ROCIM_CLIENT/src/base/LinkBuffer.cpp > CMakeFiles/main.dir/src/base/LinkBuffer.cpp.i

CMakeFiles/main.dir/src/base/LinkBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/src/base/LinkBuffer.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/ROCIM/ROCIM_CLIENT/src/base/LinkBuffer.cpp -o CMakeFiles/main.dir/src/base/LinkBuffer.cpp.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o" \
"CMakeFiles/main.dir/main.cpp.o" \
"CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

main: CMakeFiles/main.dir/src/net/LongConnectionImpl.cpp.o
main: CMakeFiles/main.dir/main.cpp.o
main: CMakeFiles/main.dir/src/base/LinkBuffer.cpp.o
main: CMakeFiles/main.dir/build.make
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libfolly.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libfollybenchmark.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libfolly_test_util.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libfolly.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libfmtd.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_context.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_filesystem.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_program_options.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_thread.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_atomic.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_chrono.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_system.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_date_time.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_container.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libboost_regex.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libdouble-conversion.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libglog.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libgflags_debug.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libevent_extrad.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libevent_pthreadsd.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libevent_cored.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libz.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libssl.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libcrypto.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libunwind.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/liblzma.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/libunwind.a
main: /root/vcpkg/vcpkg/installed/x64-linux/debug/lib/liblzma.a
main: CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/ROCIM/ROCIM_CLIENT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable main"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.dir/build: main
.PHONY : CMakeFiles/main.dir/build

CMakeFiles/main.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.dir/clean

CMakeFiles/main.dir/depend:
	cd /root/ROCIM/ROCIM_CLIENT/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/ROCIM/ROCIM_CLIENT /root/ROCIM/ROCIM_CLIENT /root/ROCIM/ROCIM_CLIENT/build /root/ROCIM/ROCIM_CLIENT/build /root/ROCIM/ROCIM_CLIENT/build/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/main.dir/depend

