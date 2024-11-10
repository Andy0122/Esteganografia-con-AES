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

    escribirImagen("salida.png", pixeles, ancho, alto);

    return 0;
}

// Read pixels
// Modify pixels
// Write pixels
