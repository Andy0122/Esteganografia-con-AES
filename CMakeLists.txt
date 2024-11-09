cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(Esteganografia_con_AES)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Añadir subdirectorios
add_subdirectory(esteganografia)

# Añadir el ejecutable
add_executable(Esteganografia_con_AES main.cpp)

# Enlazar los modulos
target_link_libraries(Esteganografia_con_AES PRIVATE Esteganografia)