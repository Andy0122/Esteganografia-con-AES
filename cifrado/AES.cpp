#include "AES.h"
#include <stdexcept>

// Definición de las constantes estáticas
const uint8_t AES::sbox[256] = {
    // 256 valores de la S-box...
    // (Por brevedad, se omiten aquí. Debes incluir los valores reales)
};

const uint8_t AES::inv_sbox[256] = {
    // 256 valores de la S-box inversa...
    // (Por brevedad, se omiten aquí. Debes incluir los valores reales)
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
