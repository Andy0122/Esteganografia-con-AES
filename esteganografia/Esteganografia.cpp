#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#include "Esteganografia.h"
#include <iostream>


bool ESTAGANOGRAFIA::cargarImagen(const std::string& nombreArchivo, std::vector<unsigned char>& pixeles, int& ancho, int& alto) {
    int canales;
    unsigned char* imagen = stbi_load(nombreArchivo.c_str(), &ancho, &alto, &canales, CANALES_RGB);
    if (!imagen) {
        std::cerr << "Error al cargar la imagen: " << nombreArchivo << std::endl;
        return false;
    }

    pixeles.assign(imagen, imagen + ancho * alto * CANALES_RGB);

    stbi_image_free(imagen);
    return true;
}

void ESTAGANOGRAFIA::escribirImagen(const std::string& nombreArchivo, const std::vector<unsigned char>& pixeles, int& ancho, int& alto) {
    if (stbi_write_png(nombreArchivo.c_str(), ancho, alto, CANALES_RGB, pixeles.data(), ancho * CANALES_RGB) == 0) {
        std::cerr << "Error al escribir la imagen: " << nombreArchivo << std::endl;
    } else {
        std::cout << "Imagen guardada correctamente: " << nombreArchivo << std::endl;
    }
}
