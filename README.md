### **Educational File Encryption Tool**

This Python script demonstrates how file encryption can be implemented using **AES (Advanced Encryption Standard)** and **Argon2** for key derivation. The tool simulates a basic ransomware attack by encrypting specific file types (`.txt`, `.docx`, `.pdf`) and creating a ransom note asking for Bitcoin payment to restore access to the files. The encryption process is done with strong cryptographic methods to mimic how ransomware typically operates.

---

### **Key Features**

- **File Encryption**: 
  - The script uses **AES** encryption (in **CFB mode**) to encrypt files.
  - Supported file types: `.txt`, `.docx`, and `.pdf`. You can extend this by modifying the code.
  
- **Key Derivation**:
  - The password entered by the user is used to derive a secure encryption key using **Argon2** (a memory-hard password hashing algorithm).
  - **Random salt** is generated for every run to enhance security.

- **Recursive File Search**:
  - The script scans the current directory and all subdirectories for the supported file types to encrypt them.
  
- **Ransom Note Generation**:
  - After encryption, the script creates a **ransom note** (in text format) informing the user about the encryption and instructing them to pay a ransom in Bitcoin to restore access to the files.
  - The note includes simulated payment instructions, with a Bitcoin address and communication details for the attacker.
  
- **Encryption Progress**:
  - The script uses **tqdm** to display the progress of encryption, showing how many files have been processed.

---

### **How It Works**

1. **Password and Key Generation**:
   - The user provides a password via the command line.
   - The password is used to generate a secure key using **Argon2**, along with a random salt. This key is used for AES encryption.

2. **File Search and Encryption**:
   - The script walks through the directory and its subdirectories.
   - It finds files with the extensions `.txt`, `.docx`, or `.pdf` that are not already encrypted (files ending with `_encrypted`).
   - Each of these files is then encrypted using AES encryption. The encrypted data is saved in a new file with the `_encrypted` suffix.

3. **Ransom Note**:
   - A ransom note is created in the current directory, advising the victim to send Bitcoin to the specified address to receive the decryption key.

---

### **Usage Instructions**

#### 1. Install Dependencies
You need to install the following dependencies:
```bash
pip install pycryptodome argon2_cffi tqdm
```

#### 2. Run the Script
To run the script, use the following command:
```bash
python script_name.py <password>
```
- Replace `<password>` with the password you want to use for encryption. The password is used to derive the encryption key.

#### 3. Check Results
- The script will create encrypted versions of the target files, appending `_encrypted` to the filename.
- A ransom note (`ransom_note.txt`) will be created in the current directory.

---

### **Example**
```bash
$ python ransomware.py mySecurePassword
```
This will encrypt all `.txt`, `.docx`, and `.pdf` files in the current directory and subdirectories, saving the encrypted versions with the `_encrypted` suffix. A ransom note will also be generated in the current directory.

---

### **Disclaimer**

**Important Warning**: This script is **for educational purposes only**. It simulates the behavior of ransomware but does not actually damage systems or steal data. Misuse of this script for malicious purposes is **illegal** and **unethical**. Deploying or using ransomware in any real-world scenario can lead to severe legal consequences.

**The authors are not responsible for any damage, loss of data, or legal issues that arise from the use or distribution of this script.** Always act within the boundaries of the law and ethical guidelines when studying or testing cybersecurity concepts.

---

### **Dependencies**

This project uses the following Python libraries:
- **`pycryptodome`**: A Python library for cryptographic operations, including AES encryption.
- **`argon2_cffi`**: For securely deriving cryptographic keys using the Argon2 password hashing algorithm.
- **`tqdm`**: A library for displaying a progress bar in the terminal.

You can install the required dependencies using `pip`:
```bash
pip install pycryptodome argon2_cffi tqdm
```

---

### **Code Overview**

- **`derive_key`**: Uses the Argon2 password hashing function to derive an encryption key from the provided password and salt.
- **`get_files_to_encrypt`**: Searches the current directory and subdirectories for `.txt`, `.docx`, and `.pdf` files that are not already encrypted.
- **`encrypt_file`**: Encrypts the file using AES in CFB mode with the derived key and a random IV.
- **`derive_encrypted_filename`**: Appends `_encrypted` to the original file name to create the encrypted file name.
- **`create_ransom_note`**: Generates a ransom note that simulates ransom instructions, including a Bitcoin address for payment.

---

### **Conclusion**

This project is a demonstration of **file encryption techniques** and the **processes used by ransomware**. It is essential to study and understand how such software works in order to better protect against malicious attacks. Always test this code in a safe, controlled environment and never use it with malicious intent.
