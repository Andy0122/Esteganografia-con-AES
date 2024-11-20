#include "esteganografia/Esteganografia.h"
#include <iostream>
#include <vector>
#include <fstream>

using namespace ESTEGANOGRAFIA;

int main() {
    const std::string rutaImagen = "C:/Users/dylan/Desktop/Esteganografia-con-AES/gato.png";
    const std::string rutaImagenSalida = "C:/Users/dylan/Desktop/Esteganografia-con-AES/imagen_oculta.png";
    const std::string rutaMensaje = "C:/Users/dylan/Desktop/Esteganografia-con-AES/mensaje.txt";
    const std::string rutaMensajeExtraido = "C:/Users/dylan/Desktop/Esteganografia-con-AES/mensaje_extraido.txt";

    std::vector<unsigned char> pixeles;
    int ancho, alto;

    // Leer la imagen
    if (!leerPixeles(rutaImagen, pixeles, ancho, alto)) {
        return -1;
    }

    // Leer el mensaje a ocultar desde un archivo
    std::vector<uint8_t> mensaje;
    std::ifstream archivoMensaje(rutaMensaje, std::ios::binary);
    if (!archivoMensaje) {
        std::cerr << "Error al abrir el archivo de mensaje: " << rutaMensaje << std::endl;
        return -1;
    }
    mensaje.assign(std::istreambuf_iterator<char>(archivoMensaje), std::istreambuf_iterator<char>());
    archivoMensaje.close();

    // Ocultar el mensaje en la imagen
    impregnarMensaje(pixeles, mensaje);

    // Escribir la imagen con el mensaje oculto
    if (!escribirPixeles(rutaImagenSalida, pixeles, ancho, alto)) {
        return -1;
    }

    // Leer la imagen con el mensaje oculto
    std::vector<unsigned char> pixelesConMensaje;
    if (!leerPixeles(rutaImagenSalida, pixelesConMensaje, ancho, alto)) {
        return -1;
    }

    // Extraer el mensaje de la imagen
    std::vector<uint8_t> mensajeExtraido;
    if (!extraerMensaje(pixelesConMensaje, mensajeExtraido)) {
        return -1;
    }

    // Escribir el mensaje extraído en un archivo
    std::ofstream archivoMensajeExtraido(rutaMensajeExtraido, std::ios::binary);
    if (!archivoMensajeExtraido) {
        std::cerr << "Error al abrir el archivo para escribir el mensaje extraído: " << rutaMensajeExtraido << std::endl;
        return -1;
    }
    archivoMensajeExtraido.write(reinterpret_cast<char*>(mensajeExtraido.data()), mensajeExtraido.size());
    archivoMensajeExtraido.close();

    std::cout << "Proceso completado exitosamente." << std::endl;

    return 0;
}
