#ifndef ESTEGANOGRAFIA_H
#define ESTEGANOGRAFIA_H

#include <string>
#include <vector>
#include <cstdint>


namespace ESTAGANOGRAFIA {
    struct RGB {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    constexpr int CANALES_RGB = 3;

    bool cargarImagen(const std::string& nombreArchivo, std::vector<unsigned char>& pixeles, int& ancho, int& alto);

} // ESTAGANOGRAFIA

#endif //ESTEGANOGRAFIA_H
