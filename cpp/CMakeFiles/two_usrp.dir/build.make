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
include CMakeFiles/two_usrp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/two_usrp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/two_usrp.dir/flags.make

CMakeFiles/two_usrp.dir/two_usrp.cpp.o: CMakeFiles/two_usrp.dir/flags.make
CMakeFiles/two_usrp.dir/two_usrp.cpp.o: two_usrp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/version2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/two_usrp.dir/two_usrp.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/two_usrp.dir/two_usrp.cpp.o -c /Users/Jens/git/B205mini-Summer/version2/two_usrp.cpp

CMakeFiles/two_usrp.dir/two_usrp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/two_usrp.dir/two_usrp.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/version2/two_usrp.cpp > CMakeFiles/two_usrp.dir/two_usrp.cpp.i

CMakeFiles/two_usrp.dir/two_usrp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/two_usrp.dir/two_usrp.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/version2/two_usrp.cpp -o CMakeFiles/two_usrp.dir/two_usrp.cpp.s

CMakeFiles/two_usrp.dir/two_usrp.cpp.o.requires:

.PHONY : CMakeFiles/two_usrp.dir/two_usrp.cpp.o.requires

CMakeFiles/two_usrp.dir/two_usrp.cpp.o.provides: CMakeFiles/two_usrp.dir/two_usrp.cpp.o.requires
	$(MAKE) -f CMakeFiles/two_usrp.dir/build.make CMakeFiles/two_usrp.dir/two_usrp.cpp.o.provides.build
.PHONY : CMakeFiles/two_usrp.dir/two_usrp.cpp.o.provides

CMakeFiles/two_usrp.dir/two_usrp.cpp.o.provides.build: CMakeFiles/two_usrp.dir/two_usrp.cpp.o


CMakeFiles/two_usrp.dir/util.cpp.o: CMakeFiles/two_usrp.dir/flags.make
CMakeFiles/two_usrp.dir/util.cpp.o: util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/version2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/two_usrp.dir/util.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/two_usrp.dir/util.cpp.o -c /Users/Jens/git/B205mini-Summer/version2/util.cpp

CMakeFiles/two_usrp.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/two_usrp.dir/util.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/version2/util.cpp > CMakeFiles/two_usrp.dir/util.cpp.i

CMakeFiles/two_usrp.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/two_usrp.dir/util.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/version2/util.cpp -o CMakeFiles/two_usrp.dir/util.cpp.s

CMakeFiles/two_usrp.dir/util.cpp.o.requires:

.PHONY : CMakeFiles/two_usrp.dir/util.cpp.o.requires

CMakeFiles/two_usrp.dir/util.cpp.o.provides: CMakeFiles/two_usrp.dir/util.cpp.o.requires
	$(MAKE) -f CMakeFiles/two_usrp.dir/build.make CMakeFiles/two_usrp.dir/util.cpp.o.provides.build
.PHONY : CMakeFiles/two_usrp.dir/util.cpp.o.provides

CMakeFiles/two_usrp.dir/util.cpp.o.provides.build: CMakeFiles/two_usrp.dir/util.cpp.o


# Object files for target two_usrp
two_usrp_OBJECTS = \
"CMakeFiles/two_usrp.dir/two_usrp.cpp.o" \
"CMakeFiles/two_usrp.dir/util.cpp.o"

# External object files for target two_usrp
two_usrp_EXTERNAL_OBJECTS =

two_usrp: CMakeFiles/two_usrp.dir/two_usrp.cpp.o
two_usrp: CMakeFiles/two_usrp.dir/util.cpp.o
two_usrp: CMakeFiles/two_usrp.dir/build.make
two_usrp: /opt/local/lib/libuhd.dylib
two_usrp: /opt/local/lib/libboost_program_options-mt.dylib
two_usrp: /opt/local/lib/libboost_system-mt.dylib
two_usrp: /opt/local/lib/libboost_filesystem-mt.dylib
two_usrp: /opt/local/lib/libboost_thread-mt.dylib
two_usrp: /opt/local/lib/libboost_chrono-mt.dylib
two_usrp: /opt/local/lib/libboost_date_time-mt.dylib
two_usrp: /opt/local/lib/libboost_atomic-mt.dylib
two_usrp: /usr/local/lib/libhidapi.dylib
two_usrp: CMakeFiles/two_usrp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Jens/git/B205mini-Summer/version2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable two_usrp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/two_usrp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/two_usrp.dir/build: two_usrp

.PHONY : CMakeFiles/two_usrp.dir/build

CMakeFiles/two_usrp.dir/requires: CMakeFiles/two_usrp.dir/two_usrp.cpp.o.requires
CMakeFiles/two_usrp.dir/requires: CMakeFiles/two_usrp.dir/util.cpp.o.requires

.PHONY : CMakeFiles/two_usrp.dir/requires

CMakeFiles/two_usrp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/two_usrp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/two_usrp.dir/clean

CMakeFiles/two_usrp.dir/depend:
	cd /Users/Jens/git/B205mini-Summer/version2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2/CMakeFiles/two_usrp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/two_usrp.dir/depend

