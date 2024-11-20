#ifndef ESTEGANOGRAFIA_H
#define ESTEGANOGRAFIA_H

#include <string>
#include <vector>
#include <cstdint>

namespace ESTEGANOGRAFIA {

 constexpr int CANALES_RGB = 3; ///< Número de canales de color (R, G, B).
 constexpr int BITS_DE_TAMANIO = 32; ///< Número de bits para almacenar el tamaño del mensaje.

 /**
  * @brief Lee los píxeles de una imagen desde un archivo PNG.
  * @param nombreArchivo Nombre del archivo de la imagen.
  * @param pixeles Vector donde se almacenarán los píxeles de la imagen.
  * @param ancho Ancho de la imagen.
  * @param alto Alto de la imagen.
  * @return true si la imagen se cargó correctamente, false en caso contrario.
  */
 bool leerPixeles(const std::string& nombreArchivo, std::vector<unsigned char>& pixeles, int& ancho, int& alto);

 /**
  * @brief Oculta un mensaje en los píxeles de la imagen.
  * @param pixeles Vector con los píxeles de la imagen.
  * @param mensaje Datos binarios del mensaje a ocultar.
  */
 void impregnarMensaje(std::vector<unsigned char>& pixeles, const std::vector<uint8_t>& mensaje);

 /**
  * @brief Extrae un mensaje oculto de los píxeles de la imagen.
  * @param pixeles Vector con los píxeles de la imagen.
  * @param mensaje Vector donde se almacenará el mensaje extraído.
  * @return true si se extrajo el mensaje correctamente, false en caso contrario.
  */
 bool extraerMensaje(const std::vector<unsigned char>& pixeles, std::vector<uint8_t>& mensaje);

 /**
  * @brief Escribe los píxeles en un archivo PNG.
  * @param nombreArchivo Nombre del archivo de la imagen.
  * @param pixeles Vector con los píxeles de la imagen.
  * @param ancho Ancho de la imagen.
  * @param alto Alto de la imagen.
  */
 bool escribirPixeles(const std::string& nombreArchivo, const std::vector<unsigned char>& pixeles, const int& ancho, const int& alto);

} // namespace ESTEGANOGRAFIA

#endif // ESTEGANOGRAFIA_H

