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
include CMakeFiles/cw05.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/cw05.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/cw05.dir/flags.make

CMakeFiles/cw05.dir/main.c.o: CMakeFiles/cw05.dir/flags.make
CMakeFiles/cw05.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/cw05.dir/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/cw05.dir/main.c.o   -c /home/student/SysOpy17/cw05/main.c

CMakeFiles/cw05.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/cw05.dir/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/student/SysOpy17/cw05/main.c > CMakeFiles/cw05.dir/main.c.i

CMakeFiles/cw05.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/cw05.dir/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/student/SysOpy17/cw05/main.c -o CMakeFiles/cw05.dir/main.c.s

CMakeFiles/cw05.dir/main.c.o.requires:

.PHONY : CMakeFiles/cw05.dir/main.c.o.requires

CMakeFiles/cw05.dir/main.c.o.provides: CMakeFiles/cw05.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/cw05.dir/build.make CMakeFiles/cw05.dir/main.c.o.provides.build
.PHONY : CMakeFiles/cw05.dir/main.c.o.provides

CMakeFiles/cw05.dir/main.c.o.provides.build: CMakeFiles/cw05.dir/main.c.o


# Object files for target cw05
cw05_OBJECTS = \
"CMakeFiles/cw05.dir/main.c.o"

# External object files for target cw05
cw05_EXTERNAL_OBJECTS =

cw05: CMakeFiles/cw05.dir/main.c.o
cw05: CMakeFiles/cw05.dir/build.make
cw05: CMakeFiles/cw05.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable cw05"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cw05.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/cw05.dir/build: cw05

.PHONY : CMakeFiles/cw05.dir/build

CMakeFiles/cw05.dir/requires: CMakeFiles/cw05.dir/main.c.o.requires

.PHONY : CMakeFiles/cw05.dir/requires

CMakeFiles/cw05.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cw05.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cw05.dir/clean

CMakeFiles/cw05.dir/depend:
	cd /home/student/SysOpy17/cw05/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/SysOpy17/cw05 /home/student/SysOpy17/cw05 /home/student/SysOpy17/cw05/cmake-build-debug /home/student/SysOpy17/cw05/cmake-build-debug /home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles/cw05.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cw05.dir/depend
