# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.9/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.9/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build

# Include any dependencies generated for this target.
include CMakeFiles/Packer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Packer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Packer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Packer.dir/flags.make

CMakeFiles/Packer.dir/src/main.cpp.o: CMakeFiles/Packer.dir/flags.make
CMakeFiles/Packer.dir/src/main.cpp.o: /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/main.cpp
CMakeFiles/Packer.dir/src/main.cpp.o: CMakeFiles/Packer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Packer.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Packer.dir/src/main.cpp.o -MF CMakeFiles/Packer.dir/src/main.cpp.o.d -o CMakeFiles/Packer.dir/src/main.cpp.o -c /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/main.cpp

CMakeFiles/Packer.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Packer.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/main.cpp > CMakeFiles/Packer.dir/src/main.cpp.i

CMakeFiles/Packer.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Packer.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/main.cpp -o CMakeFiles/Packer.dir/src/main.cpp.s

CMakeFiles/Packer.dir/src/Packer.cpp.o: CMakeFiles/Packer.dir/flags.make
CMakeFiles/Packer.dir/src/Packer.cpp.o: /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/Packer.cpp
CMakeFiles/Packer.dir/src/Packer.cpp.o: CMakeFiles/Packer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Packer.dir/src/Packer.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Packer.dir/src/Packer.cpp.o -MF CMakeFiles/Packer.dir/src/Packer.cpp.o.d -o CMakeFiles/Packer.dir/src/Packer.cpp.o -c /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/Packer.cpp

CMakeFiles/Packer.dir/src/Packer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Packer.dir/src/Packer.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/Packer.cpp > CMakeFiles/Packer.dir/src/Packer.cpp.i

CMakeFiles/Packer.dir/src/Packer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Packer.dir/src/Packer.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/Packer.cpp -o CMakeFiles/Packer.dir/src/Packer.cpp.s

CMakeFiles/Packer.dir/src/Painter.cpp.o: CMakeFiles/Packer.dir/flags.make
CMakeFiles/Packer.dir/src/Painter.cpp.o: /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/Painter.cpp
CMakeFiles/Packer.dir/src/Painter.cpp.o: CMakeFiles/Packer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Packer.dir/src/Painter.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Packer.dir/src/Painter.cpp.o -MF CMakeFiles/Packer.dir/src/Painter.cpp.o.d -o CMakeFiles/Packer.dir/src/Painter.cpp.o -c /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/Painter.cpp

CMakeFiles/Packer.dir/src/Painter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Packer.dir/src/Painter.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/Painter.cpp > CMakeFiles/Packer.dir/src/Painter.cpp.i

CMakeFiles/Packer.dir/src/Painter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Packer.dir/src/Painter.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/src/Painter.cpp -o CMakeFiles/Packer.dir/src/Painter.cpp.s

# Object files for target Packer
Packer_OBJECTS = \
"CMakeFiles/Packer.dir/src/main.cpp.o" \
"CMakeFiles/Packer.dir/src/Packer.cpp.o" \
"CMakeFiles/Packer.dir/src/Painter.cpp.o"

# External object files for target Packer
Packer_EXTERNAL_OBJECTS =

Packer: CMakeFiles/Packer.dir/src/main.cpp.o
Packer: CMakeFiles/Packer.dir/src/Packer.cpp.o
Packer: CMakeFiles/Packer.dir/src/Painter.cpp.o
Packer: CMakeFiles/Packer.dir/build.make
Packer: /opt/homebrew/opt/qt@5/lib/QtWidgets.framework/QtWidgets
Packer: /opt/homebrew/opt/qt@5/lib/QtGui.framework/QtGui
Packer: /opt/homebrew/opt/qt@5/lib/QtCore.framework/QtCore
Packer: CMakeFiles/Packer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Packer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Packer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Packer.dir/build: Packer
.PHONY : CMakeFiles/Packer.dir/build

CMakeFiles/Packer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Packer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Packer.dir/clean

CMakeFiles/Packer.dir/depend:
	cd /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build /Users/kimdonghyuk/Documents/Postech/2023-2/research/code2/PackingPlayGround/build/CMakeFiles/Packer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Packer.dir/depend

