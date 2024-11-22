#ifndef AES_H
#define AES_H

#include <vector>
#include <cstdint>

class AES {
public:
    /**
     * @brief Constructor de la clase AES.
     * @param key Clave de 16 bytes (128 bits) para el cifrado y descifrado.
     */
    AES(const std::vector<uint8_t>& key);

    /**
     * @brief Cifra un bloque de 16 bytes (128 bits).
     * @param input Bloque de entrada a cifrar.
     * @return Bloque cifrado.
     */
    std::vector<uint8_t> EncryptBlock(const std::vector<uint8_t>& input);

    /**
     * @brief Descifra un bloque de 16 bytes (128 bits).
     * @param input Bloque de entrada a descifrar.
     * @return Bloque descifrado.
     */
    std::vector<uint8_t> DecryptBlock(const std::vector<uint8_t>& input);

    /**
     * @brief Cifra datos arbitrarios utilizando relleno PKCS#7.
     * @param input Datos de entrada a cifrar.
     * @return Datos cifrados.
     */
    std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& input);

    /**
     * @brief Descifra datos cifrados utilizando relleno PKCS#7.
     * @param input Datos cifrados de entrada.
     * @return Datos descifrados.
     */
    std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& input);

private:
    static const uint8_t sbox[256];
    static const uint8_t inv_sbox[256];
    static const uint8_t rcon[11];
    std::vector<std::vector<uint8_t>> RoundKeys;

    void KeyExpansion(const std::vector<uint8_t>& key);
    void AddRoundKey(uint8_t state[4][4], const std::vector<uint8_t>& roundKey);
    void SubBytes(uint8_t state[4][4]);
    void InvSubBytes(uint8_t state[4][4]);
    void ShiftRows(uint8_t state[4][4]);
    void InvShiftRows(uint8_t state[4][4]);
    void MixColumns(uint8_t state[4][4]);
    void InvMixColumns(uint8_t state[4][4]);
    uint8_t xtime(uint8_t x);
    uint8_t multiply(uint8_t x, uint8_t y);
};

#endif // AES_H
