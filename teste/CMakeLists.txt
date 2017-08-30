set(CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabi-g++")

project(teste)
add_compile_options("-std=c++0x")
add_compile_options("-pthread")
link_libraries("-pthread")

include_directories(src_folder)
set(LIBS src_folder/ev3dev.cpp src_folder/Controlador_motor.cpp src_folder/MArquivos.cpp src_folder/Sensor_cor.cpp)
add_executable(binario  ${LIBS} main.cpp)
