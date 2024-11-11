#include "esteganografia/Esteganografia.h"
#include <iostream>


using namespace ESTAGANOGRAFIA;

int main() {
    const std::string rutaImagen = "/home/andres/Downloads/tanks.png";

    std::vector<unsigned char> pixeles;
    int ancho, alto;

    leerPixeles(rutaImagen, pixeles, ancho, alto);

    if (pixeles.empty()) {
        return -1;
    }

    impregnarMensaje(pixeles, "00111010101");

    escribirPixeles("salida.png", pixeles, ancho, alto);

    return 0;
}