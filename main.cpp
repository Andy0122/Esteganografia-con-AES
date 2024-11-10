#include "esteganografia/Esteganografia.h"
#include <iostream>


using namespace ESTAGANOGRAFIA;

int main() {
    const std::string rutaImagen = "/home/andres/Downloads/tanks.png";

    std::vector<unsigned char> pixeles;
    int ancho, alto;

    cargarImagen(rutaImagen, pixeles, ancho, alto);

    if (pixeles.empty()) {
        return -1;
    }

    // Imprimir los píxeles en formato RGB
    std::cout << "Pixels (RGB):" << std::endl;
    for (int i = 0; i < ancho * alto * CANALES_RGB; i += CANALES_RGB) {
        const unsigned char r = pixeles[i];
        const unsigned char g = pixeles[i + 1];
        const unsigned char b = pixeles[i + 2];

        // Imprimir el valor RGB
        std::cout << "R: " << static_cast<int>(r) << ", "
                  << "G: " << static_cast<int>(g) << ", "
                  << "B: " << static_cast<int>(b) << std::endl;
    }

    return 0;
}

// Copy image
// Read pixels
// Modify pixels
// Write pixels
