/**
 * Simple Encryption System Implementation
 * 
 * This file implements basic encryption functionality:
 * - String hashing for passwords
 * - File content encryption/decryption
 * - Simple XOR-based encryption
 */

#include "simpleEncryption.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

/**
 * Create a simple hash of an input string
 * 
 * Algorithm:
 * Uses DJB2 hash algorithm:
 * 1. Start with a hash value of 5381
 * 2. For each character: hash = ((hash << 5) + hash) + c
 * 
 * Used for:
 * - Password hashing
 * - Key generation
 * 
 * @param input String to hash
 * @return string Hash value as string
 */
string simpleHash(const string& input) {
    unsigned long hash = 5381;
    for (char c : input) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return to_string(hash);
}

/**
 * Encrypt/Decrypt a string using XOR
 * 
 * Process:
 * 1. XOR each character with corresponding key character
 * 2. Key is repeated if shorter than input
 * 
 * Features:
 * - Symmetric encryption (same operation encrypts and decrypts)
 * - Key-based encryption
 * 
 * @param input String to encrypt/decrypt
 * @param key Encryption key
 * @return string Encrypted/decrypted result
 */
string encryptDecrypt(const string& input, const string& key) {
    string output = input;
    for (size_t i = 0; i < input.size(); i++) {
        output[i] = input[i] ^ key[i % key.length()];
    }
    return output;
}

/**
 * Encrypt a file
 * 
 * Process:
 * 1. Read content from temporary file
 * 2. Encrypt content
 * 3. Write encrypted content to main file
 * 
 * Error Handling:
 * - Checks file accessibility
 * - Reports file operation errors
 * 
 * @param filename File to encrypt
 * @param key Encryption key
 */
void encryptFile(const string& filename, const string& key) {
    ifstream tempFile("temp_accounts.txt");
    if (!tempFile) {
        cerr << "Error: Unable to open temp_accounts.txt for reading" << endl;
        return;
    }
    string content((istreambuf_iterator<char>(tempFile)), istreambuf_iterator<char>());
    tempFile.close();

    string encrypted = encryptDecrypt(content, key);

    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Error: Unable to open " << filename << " for writing" << endl;
        return;
    }
    outFile.write(encrypted.c_str(), encrypted.size());
    outFile.close();
}

/**
 * Decrypt a file
 * 
 * Process:
 * 1. Read encrypted content
 * 2. Decrypt using key
 * 3. Return decrypted content
 * 
 * Error Handling:
 * - Verifies file existence
 * - Handles read errors
 * 
 * @param filename File to decrypt
 * @param key Decryption key
 * @return string Decrypted content
 */
string decryptFile(const string& filename, const string& key) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Error: Unable to open file for reading: " << filename << endl;
        return "";
    }
    string content((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    return encryptDecrypt(content, key);
}
