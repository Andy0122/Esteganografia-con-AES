#ifndef ESTEGANOGRAFIA_H
#define ESTEGANOGRAFIA_H

#include <string>
#include <vector>
#include <cstdint>


namespace ESTAGANOGRAFIA {

    constexpr int CANALES_RGB = 3; ///< Número de canales de color (R, G, B).
    constexpr int BITS_DE_TAMANIO = 32; ///< Número de bits para almacenar el tamaño del mensaje.

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
     * @brief Escribe el tamaño del mensaje en los primeros 32 bits de la imagen.
     * @param pixeles Vector con los píxeles de la imagen.
     * @param tamanio Tamaño del mensaje.
     */
    static void escribirTamanioMensaje(std::vector<unsigned char>& pixeles, uint32_t tamanio);

    /**
     * @brief Divide un mensaje en bloques de 3 bits.
     * @param mensaje Mensaje a dividir.
     * @param bloques Vector donde se almacenarán los bloques.
     */
    static void dividirMensajeEnBloques(const std::string& mensaje, std::vector<std::string>& bloques);

    /**
     * @brief Escribe un bit en el bit menos significativo de un canal de color.
     * @param canal Canal de color.
     * @param bit Bit a escribir.
     */
    static void escribirSobreCanal(unsigned char& canal, unsigned char bit);

    /**
     * @brief Escribe los pixeles en un archivo PNG.
     * @param nombreArchivo Nombre del archivo de la imagen.
     * @param pixeles Vector con los píxeles de la imagen.
     * @param ancho Ancho de la imagen.
     * @param alto Alto de la imagen.
     */
    void escribirPixeles(const std::string& nombreArchivo, const std::vector<unsigned char>& pixeles, const int& ancho, const int& alto);

    static void leerTamanioMensaje(const std::vector<unsigned char>& pixeles, unsigned int& tamanio);

    void extraerMensaje(const std::vector<unsigned char>& pixeles, std::vector<uint8_t> &mensaje);



} // ESTAGANOGRAFIA

#endif //ESTEGANOGRAFIA_H
