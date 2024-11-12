#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#include "Esteganografia.h"
#include <iostream>


bool ESTAGANOGRAFIA::leerPixeles(const std::string& nombreArchivo, std::vector<unsigned char>& pixeles, int& ancho, int& alto) {
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

void ESTAGANOGRAFIA::impregnarMensaje(std::vector<unsigned char>& pixeles, const std::string& mensaje) {
    if (mensaje.size() / 3 + BITS_DE_TAMANIO > pixeles.size()) {
        std::cerr << "El mensaje es demasiado grande para la imagen." << std::endl;
        return;
    }
    if (mensaje.empty()) {
        std::cerr << "El mensaje está vacío." << std::endl;
        return;
    }
    if (pixeles.empty()) {
        std::cerr << "No hay pixeles en la imagen." << std::endl;
        return;
    }

    // Escribir el tamaño del mensaje en los primeros 32 bits
    const uint32_t tamanio = mensaje.size();
    escribirTamanioMensaje(pixeles, tamanio);

    // Dividir el mensaje en bloques de 3 caracteres
    std::vector<std::string> bloques;
    dividirMensajeEnBloques(mensaje, bloques);

    int pixel = BITS_DE_TAMANIO;
    for (std::string& bloque : bloques) {
        if (bloque.size() >= 1) {
            escribirSobreCanal(pixeles[pixel], bloque[0]);
        }
        if (bloque.size() >= 2) {
            escribirSobreCanal(pixeles[pixel + 1], bloque[1]);
        }
        if (bloque.size() == 3) {
            escribirSobreCanal(pixeles[pixel + 2], bloque[2]);
        }
        pixel += 3;
    }
}

void ESTAGANOGRAFIA::escribirTamanioMensaje(std::vector<unsigned char>& pixeles, const uint32_t tamanio) {
    for (int i = 0; i < BITS_DE_TAMANIO; i++) {
        const uint8_t bit = (tamanio >> ((BITS_DE_TAMANIO - 1) - i)) & 1; // Bit shifting
        escribirSobreCanal(pixeles[i], bit);
    }
}

void ESTAGANOGRAFIA::dividirMensajeEnBloques(const std::string& mensaje, std::vector<std::string>& bloques) {
    for (size_t i = 0; i < mensaje.size(); i += 3) {
        bloques.push_back(mensaje.substr(i, 3));
    }
}

void ESTAGANOGRAFIA::escribirSobreCanal(unsigned char& canal, const unsigned char bit) {
    if (bit == '1') {
        canal |= 1; // Establecer el bit menos significativo en 1
    } else {
        canal &= ~1; // Establecer el bit menos significativo en 0
    }
}

void ESTAGANOGRAFIA::escribirPixeles(const std::string& nombreArchivo, const std::vector<unsigned char>& pixeles, const int& ancho, const int& alto) {
    if (stbi_write_png(nombreArchivo.c_str(), ancho, alto, CANALES_RGB, pixeles.data(), ancho * CANALES_RGB) == 0) {
        std::cerr << "Error al escribir la imagen: " << nombreArchivo << std::endl;
    } else {
        std::cout << "Imagen guardada correctamente: " << nombreArchivo << std::endl;
    }
}
