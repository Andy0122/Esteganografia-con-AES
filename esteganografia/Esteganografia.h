#ifndef ESTEGANOGRAFIA_H
#define ESTEGANOGRAFIA_H

#include <string>
#include <vector>
#include <cstdint>


namespace ESTAGANOGRAFIA {

    constexpr int CANALES_RGB = 3; ///< Número de canales de color (R, G, B).

    /**
     * @brief Lee los pixeles de un imagen desde un archivo PNG.
     * @param nombreArchivo Nombre del archivo de la imagen.
     * @param pixeles Vector donde se almacenarán los píxeles de la imagen.
     * @param ancho Ancho de la imagen.
     * @param alto Alto de la imagen.
     * @return true si la imagen se cargó correctamente, false en caso contrario.
     */
    bool leerPixeles(const std::string& nombreArchivo, std::vector<unsigned char>& pixeles, int& ancho, int& alto);

    /**
    * @brief Coloca un mensaje en los píxeles de la imagen.
    * @param pixeles Vector con los píxeles de la imagen.
    * @param mensaje Mensaje a impregnar en la imagen.
    */
    void impregnarMensaje(std::vector<unsigned char>& pixeles, const std::string& mensaje);

    /**
     * @brief Escribe los pixeles en un archivo PNG.
     * @param nombreArchivo Nombre del archivo de la imagen.
     * @param pixeles Vector con los píxeles de la imagen.
     * @param ancho Ancho de la imagen.
     * @param alto Alto de la imagen.
     */
    void escribirPixeles(const std::string& nombreArchivo, const std::vector<unsigned char>& pixeles, const int& ancho, const int& alto);



} // ESTAGANOGRAFIA

#endif //ESTEGANOGRAFIA_H
