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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/martin/LL1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/martin/LL1/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/sequential.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/sequential.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/sequential.dir/flags.make

examples/CMakeFiles/sequential.dir/sequential.cpp.o: examples/CMakeFiles/sequential.dir/flags.make
examples/CMakeFiles/sequential.dir/sequential.cpp.o: ../examples/sequential.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martin/LL1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/CMakeFiles/sequential.dir/sequential.cpp.o"
	cd /home/martin/LL1/build/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sequential.dir/sequential.cpp.o -c /home/martin/LL1/examples/sequential.cpp

examples/CMakeFiles/sequential.dir/sequential.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sequential.dir/sequential.cpp.i"
	cd /home/martin/LL1/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/martin/LL1/examples/sequential.cpp > CMakeFiles/sequential.dir/sequential.cpp.i

examples/CMakeFiles/sequential.dir/sequential.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sequential.dir/sequential.cpp.s"
	cd /home/martin/LL1/build/examples && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/martin/LL1/examples/sequential.cpp -o CMakeFiles/sequential.dir/sequential.cpp.s

examples/CMakeFiles/sequential.dir/sequential.cpp.o.requires:

.PHONY : examples/CMakeFiles/sequential.dir/sequential.cpp.o.requires

examples/CMakeFiles/sequential.dir/sequential.cpp.o.provides: examples/CMakeFiles/sequential.dir/sequential.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/sequential.dir/build.make examples/CMakeFiles/sequential.dir/sequential.cpp.o.provides.build
.PHONY : examples/CMakeFiles/sequential.dir/sequential.cpp.o.provides

examples/CMakeFiles/sequential.dir/sequential.cpp.o.provides.build: examples/CMakeFiles/sequential.dir/sequential.cpp.o


# Object files for target sequential
sequential_OBJECTS = \
"CMakeFiles/sequential.dir/sequential.cpp.o"

# External object files for target sequential
sequential_EXTERNAL_OBJECTS =

examples/sequential: examples/CMakeFiles/sequential.dir/sequential.cpp.o
examples/sequential: examples/CMakeFiles/sequential.dir/build.make
examples/sequential: examples/CMakeFiles/sequential.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/martin/LL1/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sequential"
	cd /home/martin/LL1/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sequential.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/sequential.dir/build: examples/sequential

.PHONY : examples/CMakeFiles/sequential.dir/build

examples/CMakeFiles/sequential.dir/requires: examples/CMakeFiles/sequential.dir/sequential.cpp.o.requires

.PHONY : examples/CMakeFiles/sequential.dir/requires

examples/CMakeFiles/sequential.dir/clean:
	cd /home/martin/LL1/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/sequential.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/sequential.dir/clean

examples/CMakeFiles/sequential.dir/depend:
	cd /home/martin/LL1/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/martin/LL1 /home/martin/LL1/examples /home/martin/LL1/build /home/martin/LL1/build/examples /home/martin/LL1/build/examples/CMakeFiles/sequential.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/sequential.dir/depend

