# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Jens/git/B205mini-Summer/signalinsamling

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Jens/git/B205mini-Summer/signalinsamling

# Include any dependencies generated for this target.
include CMakeFiles/one_usrp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/one_usrp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/one_usrp.dir/flags.make

CMakeFiles/one_usrp.dir/one_usrp.cpp.o: CMakeFiles/one_usrp.dir/flags.make
CMakeFiles/one_usrp.dir/one_usrp.cpp.o: one_usrp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/one_usrp.dir/one_usrp.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/one_usrp.dir/one_usrp.cpp.o -c /Users/Jens/git/B205mini-Summer/signalinsamling/one_usrp.cpp

CMakeFiles/one_usrp.dir/one_usrp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/one_usrp.dir/one_usrp.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/signalinsamling/one_usrp.cpp > CMakeFiles/one_usrp.dir/one_usrp.cpp.i

CMakeFiles/one_usrp.dir/one_usrp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/one_usrp.dir/one_usrp.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/signalinsamling/one_usrp.cpp -o CMakeFiles/one_usrp.dir/one_usrp.cpp.s

CMakeFiles/one_usrp.dir/one_usrp.cpp.o.requires:

.PHONY : CMakeFiles/one_usrp.dir/one_usrp.cpp.o.requires

CMakeFiles/one_usrp.dir/one_usrp.cpp.o.provides: CMakeFiles/one_usrp.dir/one_usrp.cpp.o.requires
	$(MAKE) -f CMakeFiles/one_usrp.dir/build.make CMakeFiles/one_usrp.dir/one_usrp.cpp.o.provides.build
.PHONY : CMakeFiles/one_usrp.dir/one_usrp.cpp.o.provides

CMakeFiles/one_usrp.dir/one_usrp.cpp.o.provides.build: CMakeFiles/one_usrp.dir/one_usrp.cpp.o


CMakeFiles/one_usrp.dir/util.cpp.o: CMakeFiles/one_usrp.dir/flags.make
CMakeFiles/one_usrp.dir/util.cpp.o: util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/one_usrp.dir/util.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/one_usrp.dir/util.cpp.o -c /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp

CMakeFiles/one_usrp.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/one_usrp.dir/util.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp > CMakeFiles/one_usrp.dir/util.cpp.i

CMakeFiles/one_usrp.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/one_usrp.dir/util.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/signalinsamling/util.cpp -o CMakeFiles/one_usrp.dir/util.cpp.s

CMakeFiles/one_usrp.dir/util.cpp.o.requires:

.PHONY : CMakeFiles/one_usrp.dir/util.cpp.o.requires

CMakeFiles/one_usrp.dir/util.cpp.o.provides: CMakeFiles/one_usrp.dir/util.cpp.o.requires
	$(MAKE) -f CMakeFiles/one_usrp.dir/build.make CMakeFiles/one_usrp.dir/util.cpp.o.provides.build
.PHONY : CMakeFiles/one_usrp.dir/util.cpp.o.provides

CMakeFiles/one_usrp.dir/util.cpp.o.provides.build: CMakeFiles/one_usrp.dir/util.cpp.o


# Object files for target one_usrp
one_usrp_OBJECTS = \
"CMakeFiles/one_usrp.dir/one_usrp.cpp.o" \
"CMakeFiles/one_usrp.dir/util.cpp.o"

# External object files for target one_usrp
one_usrp_EXTERNAL_OBJECTS =

one_usrp: CMakeFiles/one_usrp.dir/one_usrp.cpp.o
one_usrp: CMakeFiles/one_usrp.dir/util.cpp.o
one_usrp: CMakeFiles/one_usrp.dir/build.make
one_usrp: /opt/local/lib/libuhd.dylib
one_usrp: /opt/local/lib/libboost_program_options-mt.dylib
one_usrp: /opt/local/lib/libboost_system-mt.dylib
one_usrp: /opt/local/lib/libboost_filesystem-mt.dylib
one_usrp: /opt/local/lib/libboost_thread-mt.dylib
one_usrp: /opt/local/lib/libboost_chrono-mt.dylib
one_usrp: /opt/local/lib/libboost_date_time-mt.dylib
one_usrp: /opt/local/lib/libboost_atomic-mt.dylib
one_usrp: /usr/local/lib/libhidapi.dylib
one_usrp: CMakeFiles/one_usrp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Jens/git/B205mini-Summer/signalinsamling/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable one_usrp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/one_usrp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/one_usrp.dir/build: one_usrp

.PHONY : CMakeFiles/one_usrp.dir/build

CMakeFiles/one_usrp.dir/requires: CMakeFiles/one_usrp.dir/one_usrp.cpp.o.requires
CMakeFiles/one_usrp.dir/requires: CMakeFiles/one_usrp.dir/util.cpp.o.requires

.PHONY : CMakeFiles/one_usrp.dir/requires

CMakeFiles/one_usrp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/one_usrp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/one_usrp.dir/clean

CMakeFiles/one_usrp.dir/depend:
	cd /Users/Jens/git/B205mini-Summer/signalinsamling && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Jens/git/B205mini-Summer/signalinsamling /Users/Jens/git/B205mini-Summer/signalinsamling /Users/Jens/git/B205mini-Summer/signalinsamling /Users/Jens/git/B205mini-Summer/signalinsamling /Users/Jens/git/B205mini-Summer/signalinsamling/CMakeFiles/one_usrp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/one_usrp.dir/depend

