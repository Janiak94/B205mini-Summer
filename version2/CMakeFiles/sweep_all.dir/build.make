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
include CMakeFiles/sweep_all.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sweep_all.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sweep_all.dir/flags.make

CMakeFiles/sweep_all.dir/sweep_all.cpp.o: CMakeFiles/sweep_all.dir/flags.make
CMakeFiles/sweep_all.dir/sweep_all.cpp.o: sweep_all.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Jens/git/B205mini-Summer/version2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sweep_all.dir/sweep_all.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sweep_all.dir/sweep_all.cpp.o -c /Users/Jens/git/B205mini-Summer/version2/sweep_all.cpp

CMakeFiles/sweep_all.dir/sweep_all.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sweep_all.dir/sweep_all.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Jens/git/B205mini-Summer/version2/sweep_all.cpp > CMakeFiles/sweep_all.dir/sweep_all.cpp.i

CMakeFiles/sweep_all.dir/sweep_all.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sweep_all.dir/sweep_all.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Jens/git/B205mini-Summer/version2/sweep_all.cpp -o CMakeFiles/sweep_all.dir/sweep_all.cpp.s

CMakeFiles/sweep_all.dir/sweep_all.cpp.o.requires:

.PHONY : CMakeFiles/sweep_all.dir/sweep_all.cpp.o.requires

CMakeFiles/sweep_all.dir/sweep_all.cpp.o.provides: CMakeFiles/sweep_all.dir/sweep_all.cpp.o.requires
	$(MAKE) -f CMakeFiles/sweep_all.dir/build.make CMakeFiles/sweep_all.dir/sweep_all.cpp.o.provides.build
.PHONY : CMakeFiles/sweep_all.dir/sweep_all.cpp.o.provides

CMakeFiles/sweep_all.dir/sweep_all.cpp.o.provides.build: CMakeFiles/sweep_all.dir/sweep_all.cpp.o


# Object files for target sweep_all
sweep_all_OBJECTS = \
"CMakeFiles/sweep_all.dir/sweep_all.cpp.o"

# External object files for target sweep_all
sweep_all_EXTERNAL_OBJECTS =

sweep_all: CMakeFiles/sweep_all.dir/sweep_all.cpp.o
sweep_all: CMakeFiles/sweep_all.dir/build.make
sweep_all: /opt/local/lib/libuhd.dylib
sweep_all: /opt/local/lib/libboost_program_options-mt.dylib
sweep_all: /opt/local/lib/libboost_system-mt.dylib
sweep_all: /opt/local/lib/libboost_filesystem-mt.dylib
sweep_all: /opt/local/lib/libboost_thread-mt.dylib
sweep_all: /opt/local/lib/libboost_chrono-mt.dylib
sweep_all: /opt/local/lib/libboost_date_time-mt.dylib
sweep_all: /opt/local/lib/libboost_atomic-mt.dylib
sweep_all: /usr/local/lib/libhidapi.dylib
sweep_all: CMakeFiles/sweep_all.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Jens/git/B205mini-Summer/version2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sweep_all"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sweep_all.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sweep_all.dir/build: sweep_all

.PHONY : CMakeFiles/sweep_all.dir/build

CMakeFiles/sweep_all.dir/requires: CMakeFiles/sweep_all.dir/sweep_all.cpp.o.requires

.PHONY : CMakeFiles/sweep_all.dir/requires

CMakeFiles/sweep_all.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sweep_all.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sweep_all.dir/clean

CMakeFiles/sweep_all.dir/depend:
	cd /Users/Jens/git/B205mini-Summer/version2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2 /Users/Jens/git/B205mini-Summer/version2/CMakeFiles/sweep_all.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sweep_all.dir/depend

