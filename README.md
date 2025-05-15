# Encryption and Decryption Program

This project provides a file-based encryption and decryption tool implemented in C++. It uses custom algorithms to encode and decode text files, preserving their content while ensuring confidentiality.

## Features

- **Encryption**: Converts text in a file into an encrypted format using ASCII manipulation and a unique step-based transformation.
- **Decryption**: Reverts encrypted text back to its original form by reversing the transformation process.
- **Dynamic Steps**: Implements a dynamic encryption process that records the steps taken for each character, enhancing security.
- **User-Friendly**: Allows users to choose between encryption and decryption through a simple menu.

## Requirements

- **Operating System**: Windows
- **Compiler**: Any C++ compiler with support for C++11 or later
- **Dependencies**:
  - [OpenSSL](https://www.openssl.org/) for cryptographic functions.

## File Structure

- `a.txt`: The input file containing the text to be encrypted or decrypted.
- `temp.txt`: A temporary file used for processing data.

## How It Works

### Encryption
1. The program reads the contents of `a.txt`.
2. Each character is processed:
   - Non-space characters are transformed based on their ASCII values using a step-based system.
   - Space characters are encoded as a special symbol (`+`).
3. The encrypted content is saved back to `a.txt`.

### Decryption
1. The program reads the encrypted contents of `a.txt`.
2. Each line is processed:
   - Encrypted characters are decoded by reversing the step-based transformation.
3. The original content is restored and saved to `a.txt`.

## 🛡️ Why This Project?

This project was developed as a contribution to national digital security initiatives.  
Focusing on cryptography and image-based data encryption, it aims to inspire both the open-source community and professionals working in national intelligence and cybersecurity fields.

> 🇹🇷 I write code dedicated to the digital defense of my country.

## Usage

1. Compile the program:
   ```bash
   g++ -o encryptor main.cpp -lcrypto -lssl
