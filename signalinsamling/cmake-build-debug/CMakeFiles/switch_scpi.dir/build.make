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
include CMakeFiles/switch_scpi.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/switch_scpi.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/switch_scpi.dir/flags.make

CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o: CMakeFiles/switch_scpi.dir/flags.make
CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o: ../switch_scpi.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o -c /Users/Jens/git/B205mini-Summer/signalinsamling/switch_scpi.cpp

CMakeFiles/switch_scpi.dir/switch_scpi.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/switch_scpi.dir/switch_scpi.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/signalinsamling/switch_scpi.cpp > CMakeFiles/switch_scpi.dir/switch_scpi.cpp.i

CMakeFiles/switch_scpi.dir/switch_scpi.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/switch_scpi.dir/switch_scpi.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/signalinsamling/switch_scpi.cpp -o CMakeFiles/switch_scpi.dir/switch_scpi.cpp.s

CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o.requires:

.PHONY : CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o.requires

CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o.provides: CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o.requires
	$(MAKE) -f CMakeFiles/switch_scpi.dir/build.make CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o.provides.build
.PHONY : CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o.provides

CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o.provides.build: CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o


CMakeFiles/switch_scpi.dir/util.cpp.o: CMakeFiles/switch_scpi.dir/flags.make
CMakeFiles/switch_scpi.dir/util.cpp.o: ../util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/switch_scpi.dir/util.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/switch_scpi.dir/util.cpp.o -c /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp

CMakeFiles/switch_scpi.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/switch_scpi.dir/util.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp > CMakeFiles/switch_scpi.dir/util.cpp.i

CMakeFiles/switch_scpi.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/switch_scpi.dir/util.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp -o CMakeFiles/switch_scpi.dir/util.cpp.s

CMakeFiles/switch_scpi.dir/util.cpp.o.requires:

.PHONY : CMakeFiles/switch_scpi.dir/util.cpp.o.requires

CMakeFiles/switch_scpi.dir/util.cpp.o.provides: CMakeFiles/switch_scpi.dir/util.cpp.o.requires
	$(MAKE) -f CMakeFiles/switch_scpi.dir/build.make CMakeFiles/switch_scpi.dir/util.cpp.o.provides.build
.PHONY : CMakeFiles/switch_scpi.dir/util.cpp.o.provides

CMakeFiles/switch_scpi.dir/util.cpp.o.provides.build: CMakeFiles/switch_scpi.dir/util.cpp.o


# Object files for target switch_scpi
switch_scpi_OBJECTS = \
"CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o" \
"CMakeFiles/switch_scpi.dir/util.cpp.o"

# External object files for target switch_scpi
switch_scpi_EXTERNAL_OBJECTS =

switch_scpi: CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o
switch_scpi: CMakeFiles/switch_scpi.dir/util.cpp.o
switch_scpi: CMakeFiles/switch_scpi.dir/build.make
switch_scpi: /opt/local/lib/libuhd.dylib
switch_scpi: /opt/local/lib/libboost_program_options-mt.dylib
switch_scpi: /opt/local/lib/libboost_system-mt.dylib
switch_scpi: /opt/local/lib/libboost_filesystem-mt.dylib
switch_scpi: /opt/local/lib/libboost_thread-mt.dylib
switch_scpi: /opt/local/lib/libboost_chrono-mt.dylib
switch_scpi: /opt/local/lib/libboost_date_time-mt.dylib
switch_scpi: /opt/local/lib/libboost_atomic-mt.dylib
switch_scpi: /usr/local/lib/libhidapi.dylib
switch_scpi: CMakeFiles/switch_scpi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable switch_scpi"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/switch_scpi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/switch_scpi.dir/build: switch_scpi

.PHONY : CMakeFiles/switch_scpi.dir/build

CMakeFiles/switch_scpi.dir/requires: CMakeFiles/switch_scpi.dir/switch_scpi.cpp.o.requires
CMakeFiles/switch_scpi.dir/requires: CMakeFiles/switch_scpi.dir/util.cpp.o.requires

.PHONY : CMakeFiles/switch_scpi.dir/requires

CMakeFiles/switch_scpi.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/switch_scpi.dir/cmake_clean.cmake
.PHONY : CMakeFiles/switch_scpi.dir/clean

CMakeFiles/switch_scpi.dir/depend:
	cd /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Jens/git/B205mini-Summer/signalinsamling /Users/Jens/git/B205mini-Summer/signalinsamling /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug /Users/Jens/git/B205mini-Summer/signalinsamling/cmake-build-debug/CMakeFiles/switch_scpi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/switch_scpi.dir/depend
