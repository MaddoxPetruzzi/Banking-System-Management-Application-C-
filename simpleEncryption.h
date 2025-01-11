/**
 * Simple Encryption System Header
 * 
 * Purpose:
 * Provides basic encryption and hashing functionality for:
 * - File content encryption
 * - Password hashing
 * - Data security
 * 
 * Features:
 * - String hashing
 * - File encryption/decryption
 * - XOR-based encryption
 * 
 * Note: This is a basic encryption system for demonstration.
 * Production systems should use standard crypto libraries.
 */

#ifndef SIMPLE_ENCRYPTION_H
#define SIMPLE_ENCRYPTION_H

#include <string>

using namespace std;

// Creates a hash of the input string for password storage
string simpleHash(const string& input);

// Encrypts/decrypts a string using XOR with a key
string encryptDecrypt(const string& input, const string& key);

// Encrypts a file using the provided key
void encryptFile(const string& filename, const string& key);

// Decrypts a file using the provided key
string decryptFile(const string& filename, const string& key);

#endif // SIMPLE_ENCRYPTION_H
