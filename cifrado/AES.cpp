#include "AES.h"
#include <stdexcept>

// Definición de las constantes estáticas
const uint8_t AES::sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16

};

const uint8_t AES::inv_sbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d

};

const uint8_t AES::rcon[11] = {
    0x00, // Rcon[0] no se utiliza
    0x01, 0x02, 0x04, 0x08,
    0x10, 0x20, 0x40, 0x80,
    0x1B, 0x36
};

// Constructor
AES::AES(const std::vector<uint8_t>& key) {
    if (key.size() != 16) {
        throw std::invalid_argument("La clave debe ser de 128 bits (16 bytes).");
    }
    KeyExpansion(key);
}

// Implementación de EncryptBlock
std::vector<uint8_t> AES::EncryptBlock(const std::vector<uint8_t>& input) {
    if (input.size() != 16) {
        throw std::invalid_argument("El bloque de entrada debe ser de 16 bytes.");
    }

    uint8_t state[4][4];

    // Copiar input al estado
    for (int i = 0; i < 16; ++i) {
        state[i % 4][i / 4] = input[i];
    }

    AddRoundKey(state, RoundKeys[0]);

    for (int round = 1; round <= 9; ++round) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, RoundKeys[round]);
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, RoundKeys[10]);

    // Copiar el estado al output
    std::vector<uint8_t> output(16);
    for (int i = 0; i < 16; ++i) {
        output[i] = state[i % 4][i / 4];
    }

    return output;
}

// Implementación de DecryptBlock
std::vector<uint8_t> AES::DecryptBlock(const std::vector<uint8_t>& input) {
    if (input.size() != 16) {
        throw std::invalid_argument("El bloque de entrada debe ser de 16 bytes.");
    }

    uint8_t state[4][4];

    // Copiar input al estado
    for (int i = 0; i < 16; ++i) {
        state[i % 4][i / 4] = input[i];
    }

    AddRoundKey(state, RoundKeys[10]);

    for (int round = 9; round >= 1; --round) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, RoundKeys[round]);
        InvMixColumns(state);
    }

    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, RoundKeys[0]);

    // Copiar el estado al output
    std::vector<uint8_t> output(16);
    for (int i = 0; i < 16; ++i) {
        output[i] = state[i % 4][i / 4];
    }

    return output;
}

// Implementación de Encrypt (con relleno PKCS#7)
std::vector<uint8_t> AES::Encrypt(const std::vector<uint8_t>& input) {
    // Rellenar el input para que sea múltiplo de 16 bytes
    size_t padding = 16 - (input.size() % 16);
    std::vector<uint8_t> paddedInput = input;
    paddedInput.insert(paddedInput.end(), padding, static_cast<uint8_t>(padding));

    std::vector<uint8_t> encrypted;

    for (size_t i = 0; i < paddedInput.size(); i += 16) {
        std::vector<uint8_t> block(paddedInput.begin() + i, paddedInput.begin() + i + 16);
        std::vector<uint8_t> encryptedBlock = EncryptBlock(block);
        encrypted.insert(encrypted.end(), encryptedBlock.begin(), encryptedBlock.end());
    }

    return encrypted;
}

// Implementación de Decrypt (con eliminación del relleno PKCS#7)
std::vector<uint8_t> AES::Decrypt(const std::vector<uint8_t>& input) {
    if (input.size() % 16 != 0) {
        throw std::invalid_argument("El tamaño del input cifrado debe ser múltiplo de 16 bytes.");
    }

    std::vector<uint8_t> decrypted;

    for (size_t i = 0; i < input.size(); i += 16) {
        std::vector<uint8_t> block(input.begin() + i, input.begin() + i + 16);
        std::vector<uint8_t> decryptedBlock = DecryptBlock(block);
        decrypted.insert(decrypted.end(), decryptedBlock.begin(), decryptedBlock.end());
    }

    // Eliminar el relleno PKCS#7
    uint8_t padding = decrypted.back();
    if (padding > 16) {
        throw std::runtime_error("Error en el relleno PKCS#7.");
    }
    decrypted.resize(decrypted.size() - padding);

    return decrypted;
}

// Implementación de KeyExpansion
void AES::KeyExpansion(const std::vector<uint8_t>& key) {
    // Genera 11 claves de ronda de 16 bytes cada una
    RoundKeys.resize(11, std::vector<uint8_t>(16));

    // Copiar la clave original como primera clave de ronda
    RoundKeys[0] = key;

    for (int i = 1; i <= 10; ++i) {
        std::vector<uint8_t> temp(4);

        // Obtener las últimas 4 bytes de la clave anterior
        for (int j = 0; j < 4; ++j) {
            temp[j] = RoundKeys[i - 1][j + 12];
        }

        // RotWord
        uint8_t t = temp[0];
        temp[0] = temp[1];
        temp[1] = temp[2];
        temp[2] = temp[3];
        temp[3] = t;

        // SubBytes
        for (int j = 0; j < 4; ++j) {
            temp[j] = sbox[temp[j]];
        }

        // Rcon
        temp[0] ^= rcon[i];

        // Generar la clave de ronda
        for (int j = 0; j < 16; ++j) {
            RoundKeys[i][j] = RoundKeys[i - 1][j] ^ temp[j % 4];
            if ((j + 1) % 4 == 0 && j != 15) {
                for (int k = 0; k < 4; ++k) {
                    temp[k] = RoundKeys[i][j - 3 + k];
                }
            }
        }
    }
}

// Implementación de AddRoundKey
void AES::AddRoundKey(uint8_t state[4][4], const std::vector<uint8_t>& roundKey) {
    for (int i = 0; i < 16; ++i) {
        state[i % 4][i / 4] ^= roundKey[i];
    }
}

// Implementación de SubBytes
void AES::SubBytes(uint8_t state[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = sbox[state[i][j]];
        }
    }
}

// Implementación de InvSubBytes
void AES::InvSubBytes(uint8_t state[4][4]) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            state[i][j] = inv_sbox[state[i][j]];
        }
    }
}

// Implementación de ShiftRows
void AES::ShiftRows(uint8_t state[4][4]) {
    uint8_t temp;

    // Fila 1: shift 1 a la izquierda
    temp = state[1][0];
    for (int i = 0; i < 3; ++i) {
        state[1][i] = state[1][i + 1];
    }
    state[1][3] = temp;

    // Fila 2: shift 2 a la izquierda
    uint8_t temp1 = state[2][0];
    uint8_t temp2 = state[2][1];
    state[2][0] = state[2][2];
    state[2][1] = state[2][3];
    state[2][2] = temp1;
    state[2][3] = temp2;

    // Fila 3: shift 3 a la izquierda
    temp = state[3][3];
    for (int i = 3; i > 0; --i) {
        state[3][i] = state[3][i - 1];
    }
    state[3][0] = temp;
}

// Implementación de InvShiftRows
void AES::InvShiftRows(uint8_t state[4][4]) {
    uint8_t temp;

    // Fila 1: shift 1 a la derecha
    temp = state[1][3];
    for (int i = 3; i > 0; --i) {
        state[1][i] = state[1][i - 1];
    }
    state[1][0] = temp;

    // Fila 2: shift 2 a la derecha
    uint8_t temp1 = state[2][2];
    uint8_t temp2 = state[2][3];
    state[2][3] = state[2][1];
    state[2][2] = state[2][0];
    state[2][1] = temp2;
    state[2][0] = temp1;

    // Fila 3: shift 3 a la derecha
    temp = state[3][0];
    for (int i = 0; i < 3; ++i) {
        state[3][i] = state[3][i + 1];
    }
    state[3][3] = temp;
}

// Implementación de MixColumns
void AES::MixColumns(uint8_t state[4][4]) {
    uint8_t temp[4];
    for (int j = 0; j < 4; ++j) {
        temp[0] = multiply(0x02, state[0][j]) ^ multiply(0x03, state[1][j]) ^ state[2][j] ^ state[3][j];
        temp[1] = state[0][j] ^ multiply(0x02, state[1][j]) ^ multiply(0x03, state[2][j]) ^ state[3][j];
        temp[2] = state[0][j] ^ state[1][j] ^ multiply(0x02, state[2][j]) ^ multiply(0x03, state[3][j]);
        temp[3] = multiply(0x03, state[0][j]) ^ state[1][j] ^ state[2][j] ^ multiply(0x02, state[3][j]);

        for (int i = 0; i < 4; ++i) {
            state[i][j] = temp[i];
        }
    }
}

// Implementación de InvMixColumns
void AES::InvMixColumns(uint8_t state[4][4]) {
    uint8_t temp[4];
    for (int j = 0; j < 4; ++j) {
        temp[0] = multiply(0x0e, state[0][j]) ^ multiply(0x0b, state[1][j]) ^ multiply(0x0d, state[2][j]) ^ multiply(0x09, state[3][j]);
        temp[1] = multiply(0x09, state[0][j]) ^ multiply(0x0e, state[1][j]) ^ multiply(0x0b, state[2][j]) ^ multiply(0x0d, state[3][j]);
        temp[2] = multiply(0x0d, state[0][j]) ^ multiply(0x09, state[1][j]) ^ multiply(0x0e, state[2][j]) ^ multiply(0x0b, state[3][j]);
        temp[3] = multiply(0x0b, state[0][j]) ^ multiply(0x0d, state[1][j]) ^ multiply(0x09, state[2][j]) ^ multiply(0x0e, state[3][j]);

        for (int i = 0; i < 4; ++i) {
            state[i][j] = temp[i];
        }
    }
}

// Implementación de xtime (multiplicación por x en GF(2^8))
uint8_t AES::xtime(uint8_t x) {
    return (x << 1) ^ ((x >> 7) * 0x1b);
}

// Implementación de multiply (multiplicación en GF(2^8))
uint8_t AES::multiply(uint8_t x, uint8_t y) {
    uint8_t result = 0;
    uint8_t temp = x;

    for (int i = 0; i < 8; ++i) {
        if (y & 0x01) {
            result ^= temp;
        }
        uint8_t carry = temp & 0x80;
        temp <<= 1;
        if (carry) {
            temp ^= 0x1b;
        }
        y >>= 1;
    }

    return result;
}
