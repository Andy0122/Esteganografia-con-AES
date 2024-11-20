#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#include "Esteganografia.h"
#include <iostream>
#include <bitset>

namespace ESTEGANOGRAFIA {

    // Funciones auxiliares privadas (no expuestas en el encabezado)
    namespace {
        void escribirTamanioMensaje(std::vector<unsigned char>& pixeles, uint32_t tamanio);
        uint32_t leerTamanioMensaje(const std::vector<unsigned char>& pixeles);
        void escribirBitEnPixel(unsigned char& canal, uint8_t bit);
        uint8_t leerBitDePixel(const unsigned char& canal);
    }

    bool leerPixeles(const std::string& nombreArchivo, std::vector<unsigned char>& pixeles, int& ancho, int& alto) {
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

    void impregnarMensaje(std::vector<unsigned char>& pixeles, const std::vector<uint8_t>& mensaje) {
        size_t capacidadBits = pixeles.size(); // Un bit por canal
        size_t bitsNecesarios = BITS_DE_TAMANIO + mensaje.size() * 8;

        if (bitsNecesarios > capacidadBits) {
            std::cerr << "El mensaje es demasiado grande para la imagen." << std::endl;
            return;
        }

        if (mensaje.empty()) {
            std::cerr << "El mensaje está vacío." << std::endl;
            return;
        }

        // Escribir el tamaño del mensaje en los primeros 32 bits
        escribirTamanioMensaje(pixeles, mensaje.size());

        // Escribir el mensaje bit a bit
        size_t indicePixel = BITS_DE_TAMANIO; // Comenzar después de los bits del tamaño
        for (uint8_t byte : mensaje) {
            for (int i = 7; i >= 0; --i) {
                uint8_t bit = (byte >> i) & 1;
                escribirBitEnPixel(pixeles[indicePixel], bit);
                ++indicePixel;
            }
        }
    }

    bool extraerMensaje(const std::vector<unsigned char>& pixeles, std::vector<uint8_t>& mensaje) {
        if (pixeles.empty()) {
            std::cerr << "No hay píxeles en la imagen." << std::endl;
            return false;
        }

        // Leer el tamaño del mensaje
        uint32_t tamanioMensaje = leerTamanioMensaje(pixeles);

        size_t bitsMensaje = tamanioMensaje * 8;
        size_t capacidadBits = pixeles.size() - BITS_DE_TAMANIO;

        if (bitsMensaje > capacidadBits) {
            std::cerr << "El tamaño del mensaje es inconsistente con la imagen." << std::endl;
            return false;
        }

        // Leer el mensaje bit a bit
        mensaje.clear();
        mensaje.reserve(tamanioMensaje);

        size_t indicePixel = BITS_DE_TAMANIO; // Comenzar después de los bits del tamaño
        uint8_t byte = 0;

        for (size_t i = 0; i < bitsMensaje; ++i) {
            uint8_t bit = leerBitDePixel(pixeles[indicePixel]);
            byte = (byte << 1) | bit;

            if ((i + 1) % 8 == 0) {
                mensaje.push_back(byte);
                byte = 0;
            }

            ++indicePixel;
        }

        return true;
    }

    bool escribirPixeles(const std::string& nombreArchivo, const std::vector<unsigned char>& pixeles, const int& ancho, const int& alto) {
        if (stbi_write_png(nombreArchivo.c_str(), ancho, alto, CANALES_RGB, pixeles.data(), ancho * CANALES_RGB) == 0) {
            std::cerr << "Error al escribir la imagen: " << nombreArchivo << std::endl;
            return false;
        } else {
            std::cout << "Imagen guardada correctamente: " << nombreArchivo << std::endl;
            return true;
        }
    }

    // Implementaciones de funciones auxiliares privadas

    namespace {

        void escribirTamanioMensaje(std::vector<unsigned char>& pixeles, uint32_t tamanio) {
            for (int i = 0; i < BITS_DE_TAMANIO; ++i) {
                uint8_t bit = (tamanio >> (BITS_DE_TAMANIO - 1 - i)) & 1;
                escribirBitEnPixel(pixeles[i], bit);
            }
        }

        uint32_t leerTamanioMensaje(const std::vector<unsigned char>& pixeles) {
            uint32_t tamanio = 0;
            for (int i = 0; i < BITS_DE_TAMANIO; ++i) {
                uint8_t bit = leerBitDePixel(pixeles[i]);
                tamanio = (tamanio << 1) | bit;
            }
            return tamanio;
        }

        void escribirBitEnPixel(unsigned char& canal, uint8_t bit) {
            canal = (canal & 0xFE) | (bit & 1);
        }

        uint8_t leerBitDePixel(const unsigned char& canal) {
            return canal & 1;
        }

    } // namespace privado

} // namespace ESTEGANOGRAFIA
