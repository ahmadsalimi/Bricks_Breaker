# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.12

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.2.4\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.2.4\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\nik\Documents\GitHub\Bricks_Breaker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\nik\Documents\GitHub\Bricks_Breaker\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/project.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/project.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/project.dir/flags.make

CMakeFiles/project.dir/src/Physics.c.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/src/Physics.c.obj: CMakeFiles/project.dir/includes_C.rsp
CMakeFiles/project.dir/src/Physics.c.obj: ../src/Physics.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\nik\Documents\GitHub\Bricks_Breaker\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/project.dir/src/Physics.c.obj"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\project.dir\src\Physics.c.obj   -c C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\Physics.c

CMakeFiles/project.dir/src/Physics.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/project.dir/src/Physics.c.i"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\Physics.c > CMakeFiles\project.dir\src\Physics.c.i

CMakeFiles/project.dir/src/Physics.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/project.dir/src/Physics.c.s"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\Physics.c -o CMakeFiles\project.dir\src\Physics.c.s

CMakeFiles/project.dir/src/View.c.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/src/View.c.obj: CMakeFiles/project.dir/includes_C.rsp
CMakeFiles/project.dir/src/View.c.obj: ../src/View.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\nik\Documents\GitHub\Bricks_Breaker\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/project.dir/src/View.c.obj"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\project.dir\src\View.c.obj   -c C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\View.c

CMakeFiles/project.dir/src/View.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/project.dir/src/View.c.i"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\View.c > CMakeFiles\project.dir\src\View.c.i

CMakeFiles/project.dir/src/View.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/project.dir/src/View.c.s"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\View.c -o CMakeFiles\project.dir\src\View.c.s

CMakeFiles/project.dir/src/main.c.obj: CMakeFiles/project.dir/flags.make
CMakeFiles/project.dir/src/main.c.obj: CMakeFiles/project.dir/includes_C.rsp
CMakeFiles/project.dir/src/main.c.obj: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\nik\Documents\GitHub\Bricks_Breaker\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/project.dir/src/main.c.obj"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\project.dir\src\main.c.obj   -c C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\main.c

CMakeFiles/project.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/project.dir/src/main.c.i"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\main.c > CMakeFiles\project.dir\src\main.c.i

CMakeFiles/project.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/project.dir/src/main.c.s"
	C:\msys32\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\nik\Documents\GitHub\Bricks_Breaker\src\main.c -o CMakeFiles\project.dir\src\main.c.s

# Object files for target project
project_OBJECTS = \
"CMakeFiles/project.dir/src/Physics.c.obj" \
"CMakeFiles/project.dir/src/View.c.obj" \
"CMakeFiles/project.dir/src/main.c.obj"

# External object files for target project
project_EXTERNAL_OBJECTS =

project.exe: CMakeFiles/project.dir/src/Physics.c.obj
project.exe: CMakeFiles/project.dir/src/View.c.obj
project.exe: CMakeFiles/project.dir/src/main.c.obj
project.exe: CMakeFiles/project.dir/build.make
project.exe: ../sdl2/sdl2-lib/libSDL2.a
project.exe: ../sdl2/sdl2-lib/libSDL2main.a
project.exe: ../sdl2/sdl2-gfx-lib/libsdl-gfx.a
project.exe: CMakeFiles/project.dir/linklibs.rsp
project.exe: CMakeFiles/project.dir/objects1.rsp
project.exe: CMakeFiles/project.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\nik\Documents\GitHub\Bricks_Breaker\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable project.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\project.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/project.dir/build: project.exe

.PHONY : CMakeFiles/project.dir/build

CMakeFiles/project.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\project.dir\cmake_clean.cmake
.PHONY : CMakeFiles/project.dir/clean

CMakeFiles/project.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\nik\Documents\GitHub\Bricks_Breaker C:\Users\nik\Documents\GitHub\Bricks_Breaker C:\Users\nik\Documents\GitHub\Bricks_Breaker\cmake-build-debug C:\Users\nik\Documents\GitHub\Bricks_Breaker\cmake-build-debug C:\Users\nik\Documents\GitHub\Bricks_Breaker\cmake-build-debug\CMakeFiles\project.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/project.dir/depend

