# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

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
include CMakeFiles/binario.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/binario.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/binario.dir/flags.make

CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o: ../src_folder/ev3dev.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o -c /src/src_folder/ev3dev.cpp

CMakeFiles/binario.dir/src_folder/ev3dev.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/src_folder/ev3dev.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/src_folder/ev3dev.cpp > CMakeFiles/binario.dir/src_folder/ev3dev.cpp.i

CMakeFiles/binario.dir/src_folder/ev3dev.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/src_folder/ev3dev.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/src_folder/ev3dev.cpp -o CMakeFiles/binario.dir/src_folder/ev3dev.cpp.s

CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o.requires

CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o.provides: CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o.provides

CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o.provides.build: CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o

CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o: ../src_folder/M_arquivos.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o -c /src/src_folder/M_arquivos.cpp

CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/src_folder/M_arquivos.cpp > CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.i

CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/src_folder/M_arquivos.cpp -o CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.s

CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o.requires

CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o.provides: CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o.provides

CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o.provides.build: CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o

CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o: ../src_folder/Sensor_cor_hsv.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o -c /src/src_folder/Sensor_cor_hsv.cpp

CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/src_folder/Sensor_cor_hsv.cpp > CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.i

CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/src_folder/Sensor_cor_hsv.cpp -o CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.s

CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o.requires

CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o.provides: CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o.provides

CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o.provides.build: CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o

CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o: ../src_folder/Ultrassom_nxt.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o -c /src/src_folder/Ultrassom_nxt.cpp

CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/src_folder/Ultrassom_nxt.cpp > CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.i

CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/src_folder/Ultrassom_nxt.cpp -o CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.s

CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o.requires

CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o.provides: CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o.provides

CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o.provides.build: CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o

CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o: ../src_folder/Controlador_robo.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o -c /src/src_folder/Controlador_robo.cpp

CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/src_folder/Controlador_robo.cpp > CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.i

CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/src_folder/Controlador_robo.cpp -o CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.s

CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o.requires

CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o.provides: CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o.provides

CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o.provides.build: CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o

CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o: ../src_folder/Mapeamento.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o -c /src/src_folder/Mapeamento.cpp

CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/src_folder/Mapeamento.cpp > CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.i

CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/src_folder/Mapeamento.cpp -o CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.s

CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o.requires

CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o.provides: CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o.provides

CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o.provides.build: CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o

CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o: ../src_folder/Arquivos_mapeamento.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o -c /src/src_folder/Arquivos_mapeamento.cpp

CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/src_folder/Arquivos_mapeamento.cpp > CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.i

CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/src_folder/Arquivos_mapeamento.cpp -o CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.s

CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o.requires

CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o.provides: CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o.provides

CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o.provides.build: CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o

CMakeFiles/binario.dir/src_folder/Garra.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/src_folder/Garra.cpp.o: ../src_folder/Garra.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/src_folder/Garra.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/src_folder/Garra.cpp.o -c /src/src_folder/Garra.cpp

CMakeFiles/binario.dir/src_folder/Garra.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/src_folder/Garra.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/src_folder/Garra.cpp > CMakeFiles/binario.dir/src_folder/Garra.cpp.i

CMakeFiles/binario.dir/src_folder/Garra.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/src_folder/Garra.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/src_folder/Garra.cpp -o CMakeFiles/binario.dir/src_folder/Garra.cpp.s

CMakeFiles/binario.dir/src_folder/Garra.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/src_folder/Garra.cpp.o.requires

CMakeFiles/binario.dir/src_folder/Garra.cpp.o.provides: CMakeFiles/binario.dir/src_folder/Garra.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/src_folder/Garra.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/src_folder/Garra.cpp.o.provides

CMakeFiles/binario.dir/src_folder/Garra.cpp.o.provides.build: CMakeFiles/binario.dir/src_folder/Garra.cpp.o

CMakeFiles/binario.dir/main.cpp.o: CMakeFiles/binario.dir/flags.make
CMakeFiles/binario.dir/main.cpp.o: ../main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /src/build/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/binario.dir/main.cpp.o"
	/usr/bin/arm-linux-gnueabi-g++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/binario.dir/main.cpp.o -c /src/main.cpp

CMakeFiles/binario.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/binario.dir/main.cpp.i"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -E /src/main.cpp > CMakeFiles/binario.dir/main.cpp.i

CMakeFiles/binario.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/binario.dir/main.cpp.s"
	/usr/bin/arm-linux-gnueabi-g++  $(CXX_DEFINES) $(CXX_FLAGS) -S /src/main.cpp -o CMakeFiles/binario.dir/main.cpp.s

CMakeFiles/binario.dir/main.cpp.o.requires:
.PHONY : CMakeFiles/binario.dir/main.cpp.o.requires

CMakeFiles/binario.dir/main.cpp.o.provides: CMakeFiles/binario.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/binario.dir/build.make CMakeFiles/binario.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/binario.dir/main.cpp.o.provides

CMakeFiles/binario.dir/main.cpp.o.provides.build: CMakeFiles/binario.dir/main.cpp.o

# Object files for target binario
binario_OBJECTS = \
"CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o" \
"CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o" \
"CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o" \
"CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o" \
"CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o" \
"CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o" \
"CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o" \
"CMakeFiles/binario.dir/src_folder/Garra.cpp.o" \
"CMakeFiles/binario.dir/main.cpp.o"

# External object files for target binario
binario_EXTERNAL_OBJECTS =

binario: CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o
binario: CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o
binario: CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o
binario: CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o
binario: CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o
binario: CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o
binario: CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o
binario: CMakeFiles/binario.dir/src_folder/Garra.cpp.o
binario: CMakeFiles/binario.dir/main.cpp.o
binario: CMakeFiles/binario.dir/build.make
binario: CMakeFiles/binario.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable binario"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/binario.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/binario.dir/build: binario
.PHONY : CMakeFiles/binario.dir/build

CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/src_folder/ev3dev.cpp.o.requires
CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/src_folder/M_arquivos.cpp.o.requires
CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/src_folder/Sensor_cor_hsv.cpp.o.requires
CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/src_folder/Ultrassom_nxt.cpp.o.requires
CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/src_folder/Controlador_robo.cpp.o.requires
CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/src_folder/Mapeamento.cpp.o.requires
CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/src_folder/Arquivos_mapeamento.cpp.o.requires
CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/src_folder/Garra.cpp.o.requires
CMakeFiles/binario.dir/requires: CMakeFiles/binario.dir/main.cpp.o.requires
.PHONY : CMakeFiles/binario.dir/requires

CMakeFiles/binario.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/binario.dir/cmake_clean.cmake
.PHONY : CMakeFiles/binario.dir/clean

CMakeFiles/binario.dir/depend:
	cd /src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /src /src /src/build /src/build /src/build/CMakeFiles/binario.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/binario.dir/depend

