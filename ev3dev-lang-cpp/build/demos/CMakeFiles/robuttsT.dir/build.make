# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /src/build

# Include any dependencies generated for this target.
include demos/CMakeFiles/robuttsT.dir/depend.make

# Include the progress variables for this target.
include demos/CMakeFiles/robuttsT.dir/progress.make

# Include the compile flags for this target's objects.
include demos/CMakeFiles/robuttsT.dir/flags.make

demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o: demos/CMakeFiles/robuttsT.dir/flags.make
demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o: ../demos/RobuttsT.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o"
	cd /src/build/demos && /usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/robuttsT.dir/RobuttsT.cpp.o -c /src/demos/RobuttsT.cpp

demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/robuttsT.dir/RobuttsT.cpp.i"
	cd /src/build/demos && /usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /src/demos/RobuttsT.cpp > CMakeFiles/robuttsT.dir/RobuttsT.cpp.i

demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/robuttsT.dir/RobuttsT.cpp.s"
	cd /src/build/demos && /usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /src/demos/RobuttsT.cpp -o CMakeFiles/robuttsT.dir/RobuttsT.cpp.s

demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o.requires:

.PHONY : demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o.requires

demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o.provides: demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o.requires
	$(MAKE) -f demos/CMakeFiles/robuttsT.dir/build.make demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o.provides.build
.PHONY : demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o.provides

demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o.provides.build: demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o


# Object files for target robuttsT
robuttsT_OBJECTS = \
"CMakeFiles/robuttsT.dir/RobuttsT.cpp.o"

# External object files for target robuttsT
robuttsT_EXTERNAL_OBJECTS =

demos/robuttsT: demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o
demos/robuttsT: demos/CMakeFiles/robuttsT.dir/build.make
demos/robuttsT: libev3dev.a
demos/robuttsT: demos/CMakeFiles/robuttsT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable robuttsT"
	cd /src/build/demos && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/robuttsT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
demos/CMakeFiles/robuttsT.dir/build: demos/robuttsT

.PHONY : demos/CMakeFiles/robuttsT.dir/build

demos/CMakeFiles/robuttsT.dir/requires: demos/CMakeFiles/robuttsT.dir/RobuttsT.cpp.o.requires

.PHONY : demos/CMakeFiles/robuttsT.dir/requires

demos/CMakeFiles/robuttsT.dir/clean:
	cd /src/build/demos && $(CMAKE_COMMAND) -P CMakeFiles/robuttsT.dir/cmake_clean.cmake
.PHONY : demos/CMakeFiles/robuttsT.dir/clean

demos/CMakeFiles/robuttsT.dir/depend:
	cd /src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /src /src/demos /src/build /src/build/demos /src/build/demos/CMakeFiles/robuttsT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : demos/CMakeFiles/robuttsT.dir/depend
