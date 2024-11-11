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

void dividirMensajeEnBloques(const std::string& mensaje, std::vector<std::string>& bloques) {
    for (size_t i = 0; i < mensaje.size(); i += 3) {
        bloques.push_back(mensaje.substr(i, 3));
    }
}

void escribirSobreCanal(unsigned char& canal, const unsigned char bit) {
    if (bit == '1') {
        canal |= 1; // Establecer el bit menos significativo en 1
    } else {
        canal &= ~1; // Establecer el bit menos significativo en 0
    }
}

void ESTAGANOGRAFIA::impregnarMensaje(std::vector<unsigned char>& pixeles, const std::string& mensaje) {
    // Restricciones:
    if (mensaje.size() / 3 > pixeles.size()) {
        std::cerr << "El mensaje es demasiado grande para la imagen." << std::endl;
        return;
    }

    if (mensaje.empty()) {
        std::cerr << "El mensaje está vacío." << std::endl;
        return;
    }

    // Dividir el mensaje en bloques de 3 caracteres
    std::vector<std::string> bloques;
    dividirMensajeEnBloques(mensaje, bloques);

    // Inicializar el indice de los pixeles
    int pixel = 0;
    // Iterar sobre los bloques de 3 caracteres
    for (std::string& bloque : bloques) {
        // Si el tamaño del bloque mayor o igual a 1
        if (bloque.size() >= 1) {
            // escribir el primer bit en el canal R
            escribirSobreCanal(pixeles[pixel], bloque[0]);
        }
        // Si el tamaño del bloque mayor o igual a 2
        if (bloque.size() >= 2) {
            // escribir el segundo bit en el canal G
            escribirSobreCanal(pixeles[pixel + 1], bloque[1]);
        }
        // Si el tamaño del bloque mayor o igual a 3
        if (bloque.size() == 3) {
            // escribir el tercer bit en el canal B
            escribirSobreCanal(pixeles[pixel + 2], bloque[2]);
        }
        // Incrementar el indice de los pixeles
        pixel += 3;
    }
}

void ESTAGANOGRAFIA::escribirPixeles(const std::string& nombreArchivo, const std::vector<unsigned char>& pixeles, const int& ancho, const int& alto) {
    if (stbi_write_png(nombreArchivo.c_str(), ancho, alto, CANALES_RGB, pixeles.data(), ancho * CANALES_RGB) == 0) {
        std::cerr << "Error al escribir la imagen: " << nombreArchivo << std::endl;
    } else {
        std::cout << "Imagen guardada correctamente: " << nombreArchivo << std::endl;
    }
}
