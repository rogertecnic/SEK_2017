set(CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabi-g++")

project(teste)
add_compile_options("-std=c++0x")
add_compile_options("-pthread")
link_libraries("-pthread")


add_executable(binario ev3dev.cpp Motorcontrolado.cpp Main.cpp)
