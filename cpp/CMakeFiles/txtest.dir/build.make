# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/IDE/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/IDE/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Jens/git/B205mini-Summer/version2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Jens/git/B205mini-Summer/version2

# Include any dependencies generated for this target.
include CMakeFiles/txtest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/txtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/txtest.dir/flags.make

CMakeFiles/txtest.dir/txtest.cpp.o: CMakeFiles/txtest.dir/flags.make
CMakeFiles/txtest.dir/txtest.cpp.o: txtest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/version2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/txtest.dir/txtest.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/txtest.dir/txtest.cpp.o -c /Users/Jens/git/B205mini-Summer/version2/txtest.cpp

CMakeFiles/txtest.dir/txtest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/txtest.dir/txtest.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/version2/txtest.cpp > CMakeFiles/txtest.dir/txtest.cpp.i

CMakeFiles/txtest.dir/txtest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/txtest.dir/txtest.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/version2/txtest.cpp -o CMakeFiles/txtest.dir/txtest.cpp.s

CMakeFiles/txtest.dir/txtest.cpp.o.requires:

.PHONY : CMakeFiles/txtest.dir/txtest.cpp.o.requires

CMakeFiles/txtest.dir/txtest.cpp.o.provides: CMakeFiles/txtest.dir/txtest.cpp.o.requires
	$(MAKE) -f CMakeFiles/txtest.dir/build.make CMakeFiles/txtest.dir/txtest.cpp.o.provides.build
.PHONY : CMakeFiles/txtest.dir/txtest.cpp.o.provides

CMakeFiles/txtest.dir/txtest.cpp.o.provides.build: CMakeFiles/txtest.dir/txtest.cpp.o


# Object files for target txtest
txtest_OBJECTS = \
"CMakeFiles/txtest.dir/txtest.cpp.o"

# External object files for target txtest
txtest_EXTERNAL_OBJECTS =

txtest: CMakeFiles/txtest.dir/txtest.cpp.o
txtest: CMakeFiles/txtest.dir/build.make
txtest: /opt/local/lib/libuhd.dylib
txtest: /opt/local/lib/libboost_program_options-mt.dylib
txtest: /opt/local/lib/libboost_system-mt.dylib
txtest: /opt/local/lib/libboost_filesystem-mt.dylib
txtest: /opt/local/lib/libboost_thread-mt.dylib
txtest: /opt/local/lib/libboost_chrono-mt.dylib
txtest: /opt/local/lib/libboost_date_time-mt.dylib
txtest: /opt/local/lib/libboost_atomic-mt.dylib
txtest: /usr/local/lib/libhidapi.dylib
txtest: CMakeFiles/txtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Jens/git/B205mini-Summer/version2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable txtest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/txtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/txtest.dir/build: txtest

.PHONY : CMakeFiles/txtest.dir/build

CMakeFiles/txtest.dir/requires: CMakeFiles/txtest.dir/txtest.cpp.o.requires

.PHONY : CMakeFiles/txtest.dir/requires

CMakeFiles/txtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/txtest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/txtest.dir/clean

CMakeFiles/txtest.dir/depend:
	cd /Users/Jens/git/B205mini-Summer/version2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2/CMakeFiles/txtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/txtest.dir/depend
