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
include demos/CMakeFiles/remote_control-test.dir/depend.make

# Include the progress variables for this target.
include demos/CMakeFiles/remote_control-test.dir/progress.make

# Include the compile flags for this target's objects.
include demos/CMakeFiles/remote_control-test.dir/flags.make

demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o: demos/CMakeFiles/remote_control-test.dir/flags.make
demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o: ../demos/remote_control-test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o"
	cd /src/build/demos && /usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o -c /src/demos/remote_control-test.cpp

demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/remote_control-test.dir/remote_control-test.cpp.i"
	cd /src/build/demos && /usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /src/demos/remote_control-test.cpp > CMakeFiles/remote_control-test.dir/remote_control-test.cpp.i

demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/remote_control-test.dir/remote_control-test.cpp.s"
	cd /src/build/demos && /usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /src/demos/remote_control-test.cpp -o CMakeFiles/remote_control-test.dir/remote_control-test.cpp.s

demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o.requires:

.PHONY : demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o.requires

demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o.provides: demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o.requires
	$(MAKE) -f demos/CMakeFiles/remote_control-test.dir/build.make demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o.provides.build
.PHONY : demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o.provides

demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o.provides.build: demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o


# Object files for target remote_control-test
remote_control__test_OBJECTS = \
"CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o"

# External object files for target remote_control-test
remote_control__test_EXTERNAL_OBJECTS =

demos/remote_control-test: demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o
demos/remote_control-test: demos/CMakeFiles/remote_control-test.dir/build.make
demos/remote_control-test: libev3dev.a
demos/remote_control-test: demos/CMakeFiles/remote_control-test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable remote_control-test"
	cd /src/build/demos && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/remote_control-test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
demos/CMakeFiles/remote_control-test.dir/build: demos/remote_control-test

.PHONY : demos/CMakeFiles/remote_control-test.dir/build

demos/CMakeFiles/remote_control-test.dir/requires: demos/CMakeFiles/remote_control-test.dir/remote_control-test.cpp.o.requires

.PHONY : demos/CMakeFiles/remote_control-test.dir/requires

demos/CMakeFiles/remote_control-test.dir/clean:
	cd /src/build/demos && $(CMAKE_COMMAND) -P CMakeFiles/remote_control-test.dir/cmake_clean.cmake
.PHONY : demos/CMakeFiles/remote_control-test.dir/clean

demos/CMakeFiles/remote_control-test.dir/depend:
	cd /src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /src /src/demos /src/build /src/build/demos /src/build/demos/CMakeFiles/remote_control-test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : demos/CMakeFiles/remote_control-test.dir/depend

