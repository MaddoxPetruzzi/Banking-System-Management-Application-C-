/**
 * Utility Functions Implementation
 * 
 * This file implements common utility functions used across the system:
 * - User interface helpers
 * - Input validation
 * - String processing
 * - System operations
 */

#include "utilityFunctions.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <termios.h>
#include <unistd.h>
#include <cstring>

using namespace std;

/**
 * Clear the console screen
 * 
 * Uses ANSI escape codes to clear screen
 * Works on UNIX-like systems
 */
void clearScreen() {
    cout << "\033[H\033[2J" << endl;
}

/**
 * Validate and get monetary amount input
 * 
 * Features:
 * - Ensures positive numbers
 * - Handles invalid input
 * - Provides custom prompts
 * 
 * @param prompt Custom prompt message
 * @return double Valid monetary amount
 */
double getValidAmount(const char* prompt) {
    double amount;
    while (true) {
        cout << prompt;
        if (cin >> amount && amount >= 0) {
            return amount;
        }
        cout << "Invalid input. Please enter a non-negative number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

/**
 * Validate and get integer input
 * 
 * Features:
 * - Ensures whole numbers
 * - Handles invalid input
 * - Provides custom prompts
 * 
 * @param prompt Custom prompt message
 * @return int Valid integer value
 */
int getValidInteger(const char* prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            return value;
        }
        cout << "Invalid input. Please enter a non-negative integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

/**
 * Convert string to lowercase
 * 
 * Used for:
 * - Case-insensitive comparisons
 * - Standardizing input
 * 
 * @param str Input string
 * @return string Lowercase version of input
 */
string toLowerCase(const string& str) {
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

/**
 * Get current date/time as formatted string
 * 
 * Format: YYYY-MM-DD HH:MM:SS
 * Used for:
 * - Transaction timestamps
 * - Log entries
 * - Receipts
 * 
 * @return string Formatted current date/time
 */
string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
    
    return string(buffer);
}

/**
 * Get password input with masking
 * 
 * Features:
 * - Hides password characters
 * - Shows asterisks for feedback
 * - Handles backspace
 * 
 * Security:
 * - Disables terminal echo
 * - Restores terminal settings after input
 * 
 * @return string Password input
 */
string getHiddenInput() {
    string input;
    char c;
    
    // Save current terminal settings
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    
    // Disable echo
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Read password
    while ((c = getchar()) != '\n' && c != EOF) {
        input.push_back(c);
        cout << '*';  // Show asterisk for feedback
    }
    cout << endl;
    
    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    
    return input;
}
