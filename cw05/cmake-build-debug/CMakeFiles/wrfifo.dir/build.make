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
include CMakeFiles/wrfifo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/wrfifo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wrfifo.dir/flags.make

CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o: CMakeFiles/wrfifo.dir/flags.make
CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o: ../zad2/wrfifo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o   -c /home/student/SysOpy17/cw05/zad2/wrfifo.c

CMakeFiles/wrfifo.dir/zad2/wrfifo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/wrfifo.dir/zad2/wrfifo.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/student/SysOpy17/cw05/zad2/wrfifo.c > CMakeFiles/wrfifo.dir/zad2/wrfifo.c.i

CMakeFiles/wrfifo.dir/zad2/wrfifo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/wrfifo.dir/zad2/wrfifo.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/student/SysOpy17/cw05/zad2/wrfifo.c -o CMakeFiles/wrfifo.dir/zad2/wrfifo.c.s

CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o.requires:

.PHONY : CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o.requires

CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o.provides: CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o.requires
	$(MAKE) -f CMakeFiles/wrfifo.dir/build.make CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o.provides.build
.PHONY : CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o.provides

CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o.provides.build: CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o


# Object files for target wrfifo
wrfifo_OBJECTS = \
"CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o"

# External object files for target wrfifo
wrfifo_EXTERNAL_OBJECTS =

wrfifo: CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o
wrfifo: CMakeFiles/wrfifo.dir/build.make
wrfifo: CMakeFiles/wrfifo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable wrfifo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wrfifo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wrfifo.dir/build: wrfifo

.PHONY : CMakeFiles/wrfifo.dir/build

CMakeFiles/wrfifo.dir/requires: CMakeFiles/wrfifo.dir/zad2/wrfifo.c.o.requires

.PHONY : CMakeFiles/wrfifo.dir/requires

CMakeFiles/wrfifo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wrfifo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wrfifo.dir/clean

CMakeFiles/wrfifo.dir/depend:
	cd /home/student/SysOpy17/cw05/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/SysOpy17/cw05 /home/student/SysOpy17/cw05 /home/student/SysOpy17/cw05/cmake-build-debug /home/student/SysOpy17/cw05/cmake-build-debug /home/student/SysOpy17/cw05/cmake-build-debug/CMakeFiles/wrfifo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wrfifo.dir/depend

