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
CMAKE_SOURCE_DIR = /Users/Jens/git/B205mini-Summer/signalinsamling

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hid_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hid_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hid_test.dir/flags.make

CMakeFiles/hid_test.dir/hid_test.cpp.o: CMakeFiles/hid_test.dir/flags.make
CMakeFiles/hid_test.dir/hid_test.cpp.o: ../hid_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hid_test.dir/hid_test.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hid_test.dir/hid_test.cpp.o -c /Users/Jens/git/B205mini-Summer/signalinsamling/hid_test.cpp

CMakeFiles/hid_test.dir/hid_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hid_test.dir/hid_test.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/signalinsamling/hid_test.cpp > CMakeFiles/hid_test.dir/hid_test.cpp.i

CMakeFiles/hid_test.dir/hid_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hid_test.dir/hid_test.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/signalinsamling/hid_test.cpp -o CMakeFiles/hid_test.dir/hid_test.cpp.s

CMakeFiles/hid_test.dir/hid_test.cpp.o.requires:

.PHONY : CMakeFiles/hid_test.dir/hid_test.cpp.o.requires

CMakeFiles/hid_test.dir/hid_test.cpp.o.provides: CMakeFiles/hid_test.dir/hid_test.cpp.o.requires
	$(MAKE) -f CMakeFiles/hid_test.dir/build.make CMakeFiles/hid_test.dir/hid_test.cpp.o.provides.build
.PHONY : CMakeFiles/hid_test.dir/hid_test.cpp.o.provides

CMakeFiles/hid_test.dir/hid_test.cpp.o.provides.build: CMakeFiles/hid_test.dir/hid_test.cpp.o


CMakeFiles/hid_test.dir/util.cpp.o: CMakeFiles/hid_test.dir/flags.make
CMakeFiles/hid_test.dir/util.cpp.o: ../util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/hid_test.dir/util.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hid_test.dir/util.cpp.o -c /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp

CMakeFiles/hid_test.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hid_test.dir/util.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp > CMakeFiles/hid_test.dir/util.cpp.i

CMakeFiles/hid_test.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hid_test.dir/util.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp -o CMakeFiles/hid_test.dir/util.cpp.s

CMakeFiles/hid_test.dir/util.cpp.o.requires:

.PHONY : CMakeFiles/hid_test.dir/util.cpp.o.requires

CMakeFiles/hid_test.dir/util.cpp.o.provides: CMakeFiles/hid_test.dir/util.cpp.o.requires
	$(MAKE) -f CMakeFiles/hid_test.dir/build.make CMakeFiles/hid_test.dir/util.cpp.o.provides.build
.PHONY : CMakeFiles/hid_test.dir/util.cpp.o.provides

CMakeFiles/hid_test.dir/util.cpp.o.provides.build: CMakeFiles/hid_test.dir/util.cpp.o


# Object files for target hid_test
hid_test_OBJECTS = \
"CMakeFiles/hid_test.dir/hid_test.cpp.o" \
"CMakeFiles/hid_test.dir/util.cpp.o"

# External object files for target hid_test
hid_test_EXTERNAL_OBJECTS =

hid_test: CMakeFiles/hid_test.dir/hid_test.cpp.o
hid_test: CMakeFiles/hid_test.dir/util.cpp.o
hid_test: CMakeFiles/hid_test.dir/build.make
hid_test: /opt/local/lib/libuhd.dylib
hid_test: /opt/local/lib/libboost_program_options-mt.dylib
hid_test: /opt/local/lib/libboost_system-mt.dylib
hid_test: /opt/local/lib/libboost_filesystem-mt.dylib
hid_test: /opt/local/lib/libboost_thread-mt.dylib
hid_test: /opt/local/lib/libboost_chrono-mt.dylib
hid_test: /opt/local/lib/libboost_date_time-mt.dylib
hid_test: /opt/local/lib/libboost_atomic-mt.dylib
hid_test: /usr/local/lib/libhidapi.dylib
hid_test: CMakeFiles/hid_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable hid_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hid_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hid_test.dir/build: hid_test

.PHONY : CMakeFiles/hid_test.dir/build

CMakeFiles/hid_test.dir/requires: CMakeFiles/hid_test.dir/hid_test.cpp.o.requires
CMakeFiles/hid_test.dir/requires: CMakeFiles/hid_test.dir/util.cpp.o.requires

.PHONY : CMakeFiles/hid_test.dir/requires

CMakeFiles/hid_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hid_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hid_test.dir/clean

CMakeFiles/hid_test.dir/depend:
	cd /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Jens/git/B205mini-Summer/signalinsamling /Users/Jens/git/B205mini-Summer/signalinsamling /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug/CMakeFiles/hid_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hid_test.dir/depend

