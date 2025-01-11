/**
 * Utility Functions Header
 * 
 * Purpose:
 * Provides common utility functions used throughout the banking system.
 * Includes screen management, input validation, and formatting.
 * 
 * Features:
 * - Screen clearing
 * - Input validation
 * - String manipulation
 * - Date/time handling
 * - Password input masking
 */

#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <string>

using namespace std;

// Screen management
void clearScreen();

// Input validation
double getValidAmount(const char* prompt = "Enter amount: $");
int getValidInteger(const char* prompt = "Enter value: ");

// String manipulation
string toLowerCase(const string& str);

// Date/time handling
string getCurrentDate();

// Secure input
string getHiddenInput();

#endif // UTILITY_FUNCTIONS_H
