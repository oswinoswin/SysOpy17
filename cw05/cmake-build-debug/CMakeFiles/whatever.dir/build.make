# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

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
CMAKE_COMMAND = /opt/clion-2016.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /opt/clion-2016.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/student/SysOpy17/cw05

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/student/SysOpy17/cw05/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/whatever.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/whatever.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/whatever.dir/flags.make

CMakeFiles/whatever.dir/zad1/interpreter2.c.o: CMakeFiles/whatever.dir/flags.make
CMakeFiles/whatever.dir/zad1/interpreter2.c.o: ../zad1/interpreter2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/whatever.dir/zad1/interpreter2.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/whatever.dir/zad1/interpreter2.c.o   -c /home/student/SysOpy17/cw05/zad1/interpreter2.c

CMakeFiles/whatever.dir/zad1/interpreter2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/whatever.dir/zad1/interpreter2.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/student/SysOpy17/cw05/zad1/interpreter2.c > CMakeFiles/whatever.dir/zad1/interpreter2.c.i

CMakeFiles/whatever.dir/zad1/interpreter2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/whatever.dir/zad1/interpreter2.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/student/SysOpy17/cw05/zad1/interpreter2.c -o CMakeFiles/whatever.dir/zad1/interpreter2.c.s

CMakeFiles/whatever.dir/zad1/interpreter2.c.o.requires:

.PHONY : CMakeFiles/whatever.dir/zad1/interpreter2.c.o.requires

CMakeFiles/whatever.dir/zad1/interpreter2.c.o.provides: CMakeFiles/whatever.dir/zad1/interpreter2.c.o.requires
	$(MAKE) -f CMakeFiles/whatever.dir/build.make CMakeFiles/whatever.dir/zad1/interpreter2.c.o.provides.build
.PHONY : CMakeFiles/whatever.dir/zad1/interpreter2.c.o.provides

CMakeFiles/whatever.dir/zad1/interpreter2.c.o.provides.build: CMakeFiles/whatever.dir/zad1/interpreter2.c.o


# Object files for target whatever
whatever_OBJECTS = \
"CMakeFiles/whatever.dir/zad1/interpreter2.c.o"

# External object files for target whatever
whatever_EXTERNAL_OBJECTS =

whatever: CMakeFiles/whatever.dir/zad1/interpreter2.c.o
whatever: CMakeFiles/whatever.dir/build.make
whatever: CMakeFiles/whatever.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable whatever"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/whatever.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/whatever.dir/build: whatever

.PHONY : CMakeFiles/whatever.dir/build

CMakeFiles/whatever.dir/requires: CMakeFiles/whatever.dir/zad1/interpreter2.c.o.requires

.PHONY : CMakeFiles/whatever.dir/requires

CMakeFiles/whatever.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/whatever.dir/cmake_clean.cmake
.PHONY : CMakeFiles/whatever.dir/clean

CMakeFiles/whatever.dir/depend:
	cd /home/student/SysOpy17/cw05/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/SysOpy17/cw05 /home/student/SysOpy17/cw05 /home/student/SysOpy17/cw05/cmake-build-debug /home/student/SysOpy17/cw05/cmake-build-debug /home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles/whatever.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/whatever.dir/depend
