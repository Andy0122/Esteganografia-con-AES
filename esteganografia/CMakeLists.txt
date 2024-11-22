# Añadir el módulo Esteganografia
add_library(Esteganografia Esteganografia.cpp)

# Incluir encabezados de stb_image
target_include_directories(Esteganografia PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Definir STB_IMAGE_IMPLEMENTATION para habilitar la implementación
target_compile_definitions(Esteganografia PRIVATE STB_IMAGE_IMPLEMENTATION)

# Enlazar stb_image con Esteganografia
target_sources(Esteganografia PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/stb_image.h
        ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/stb_image_write.h
)