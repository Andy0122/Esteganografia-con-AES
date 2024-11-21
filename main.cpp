#include "esteganografia/Esteganografia.h"
#include "cifrado/AES.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace ESTEGANOGRAFIA;

void ocultarMensaje(const std::string& rutaImagen, const std::string& rutaImagenSalida, const std::string& rutaMensaje, const std::vector<uint8_t>& clave);
void extraerMensaje(const std::string& rutaImagen, const std::string& rutaMensajeSalida, const std::vector<uint8_t>& clave);

int main() {
    std::cout << "=== Sistema de Esteganografía con Cifrado AES ===" << std::endl;
    std::cout << "Seleccione una opción:" << std::endl;
    std::cout << "1. Ocultar mensaje" << std::endl;
    std::cout << "2. Extraer mensaje" << std::endl;

    int opcion;
    std::cin >> opcion;

    std::string rutaImagen, rutaImagenSalida, rutaMensaje, rutaMensajeSalida;
    std::vector<uint8_t> clave;

    // Solicitar clave
    std::string claveStr;
    std::cout << "Ingrese la clave (16 caracteres): ";
    std::cin >> claveStr;

    if (claveStr.size() != 16) {
        std::cerr << "Error: La clave debe ser exactamente de 16 caracteres." << std::endl;
        return -1;
    }
    clave.assign(claveStr.begin(), claveStr.end());

    if (opcion == 1) {
        // Ocultar mensaje
        std::cout << "Ruta de la imagen PNG original: ";
        std::cin >> rutaImagen;

        std::cout << "Ruta del archivo de mensaje a ocultar: ";
        std::cin >> rutaMensaje;

        std::cout << "Ruta de la imagen de salida (con mensaje oculto): ";
        std::cin >> rutaImagenSalida;

        ocultarMensaje(rutaImagen, rutaImagenSalida, rutaMensaje, clave);

    } else if (opcion == 2) {
        // Extraer mensaje
        std::cout << "Ruta de la imagen PNG con mensaje oculto: ";
        std::cin >> rutaImagen;

        std::cout << "Ruta del archivo donde guardar el mensaje extraído: ";
        std::cin >> rutaMensajeSalida;

        extraerMensaje(rutaImagen, rutaMensajeSalida, clave);

    } else {
        std::cerr << "Opción no válida." << std::endl;
        return -1;
    }

    return 0;
}

void ocultarMensaje(const std::string& rutaImagen, const std::string& rutaImagenSalida, const std::string& rutaMensaje, const std::vector<uint8_t>& clave) {
    AES aes(clave);

    // Leer el mensaje a cifrar
    std::vector<uint8_t> mensaje;
    std::ifstream archivoMensaje(rutaMensaje, std::ios::binary);
    if (!archivoMensaje) {
        std::cerr << "Error al abrir el archivo de mensaje: " << rutaMensaje << std::endl;
        return;
    }
    mensaje.assign(std::istreambuf_iterator<char>(archivoMensaje), std::istreambuf_iterator<char>());
    archivoMensaje.close();

    // Cifrar el mensaje
    std::vector<uint8_t> mensajeCifrado = aes.Encrypt(mensaje);

    // Leer la imagen
    std::vector<unsigned char> pixeles;
    int ancho, alto;
    if (!leerPixeles(rutaImagen, pixeles, ancho, alto)) {
        return;
    }

    // Ocultar el mensaje cifrado en la imagen
    impregnarMensaje(pixeles, mensajeCifrado);

    // Escribir la imagen con el mensaje oculto
    if (!escribirPixeles(rutaImagenSalida, pixeles, ancho, alto)) {
        return;
    }

    std::cout << "Mensaje ocultado correctamente en la imagen: " << rutaImagenSalida << std::endl;
}

void extraerMensaje(const std::string& rutaImagen, const std::string& rutaMensajeSalida, const std::vector<uint8_t>& clave) {
    AES aes(clave);

    // Leer la imagen con el mensaje oculto
    std::vector<unsigned char> pixeles;
    int ancho, alto;
    if (!leerPixeles(rutaImagen, pixeles, ancho, alto)) {
        return;
    }

    // Extraer el mensaje cifrado de la imagen
    std::vector<uint8_t> mensajeCifrado;
    if (!extraerMensaje(pixeles, mensajeCifrado)) {
        return;
    }

    // Descifrar el mensaje
    std::vector<uint8_t> mensajeDescifrado = aes.Decrypt(mensajeCifrado);

    // Escribir el mensaje descifrado en un archivo
    std::ofstream archivoMensajeExtraido(rutaMensajeSalida, std::ios::binary);
    if (!archivoMensajeExtraido) {
        std::cerr << "Error al abrir el archivo para escribir el mensaje extraído: " << rutaMensajeSalida << std::endl;
        return;
    }
    archivoMensajeExtraido.write(reinterpret_cast<char*>(mensajeDescifrado.data()), mensajeDescifrado.size());
    archivoMensajeExtraido.close();

    std::cout << "Mensaje extraído y descifrado correctamente en: " << rutaMensajeSalida << std::endl;
}

