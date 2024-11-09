# Archivos fuente del módulo de esteganografía
SET(SOURCE_FILES
        src/Esteganografia.cpp
)

# Añadir el directorio de headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Añadir los archivos fuente del módulo de esteganografía
add_library(Esteganografia STATIC ${SOURCE_FILES})