set(CMAKE_C_COMPILER "arm-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabi-g++")

project(teste)

#diz ao compilador para usar o padrao c++11
add_compile_options("-std=c++0x")

# diz ao compilador e o linker que exite thread no codigo
add_compile_options("-pthread")
link_libraries("-pthread")

# inclui o diretorio para busca dos sources
include_directories(src_folder)

# seta a variavel uma lista com a biblioteca padrao
set(LIBS src_folder/ev3dev.cpp)

# adiciona a variavel todos os sources do nosso codigo
list(APPEND LIBS  src_folder/Controlador_motor.cpp)
list(APPEND LIBS  src_folder/M_arquivos.cpp)
list(APPEND LIBS  src_folder/Sensor_cor.cpp)
list(APPEND LIBS  src_folder/Sensor_cor_hsv.cpp)
list(APPEND LIBS  src_folder/Ultrassom_nxt.cpp)
list(APPEND LIBS  src_folder/Controlador_robo.cpp)
list(APPEND LIBS  src_folder/Mapeamento.cpp)
list(APPEND LIBS  src_folder/Arquivos_mapeamento.cpp)

# adiciona todos os sources e o main para serem compilados juntos
add_executable(binario  ${LIBS} main.cpp)
