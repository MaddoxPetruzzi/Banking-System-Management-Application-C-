/**
 * Banking System Main Program
 * 
 * Purpose:
 * Entry point for the banking system application.
 * Handles:
 * - User login
 * - Menu navigation
 * - Role-based access control
 * - Session management
 */

#include <iostream>
#include <string>
#include "menuFunctions.h"
#include "userManagement.h"
#include "utilityFunctions.h"

// Color codes for terminal output formatting
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

using namespace std;

/**
 * Main program entry point
 *
 * Program Flow:
 * 1. Display login screen
 * 2. Authenticate user
 * 3. Direct to appropriate menu based on role
 * 4. Handle session until logout
 *
 * Features:
 * - Continuous login loop until exit
 * - Role-based menu routing
 * - Error handling for invalid credentials
 * - Clean exit functionality
 *
 * @return int Program exit status
 */
int main() {
    string username, password;
    
    // Main program loop
    while (true) {
        // Display login screen and get credentials
        displayLoginMenu(username, password);
        
        // Check for exit condition
        if (username == "exit") {
            cout << "Thank you for using BankSystem. Goodbye!\n";
            break;
        }
        
        // Authenticate and route to appropriate menu
        if (authenticateUser(username, password)) {
            User currentUser = getCurrentUser();
            if (currentUser.role == UserRole::MANAGER) {
                // Manager menu access
                displayManagerMenu();
            } else {
                // Client menu access with their account numbers
                displayClientMenu(currentUser.accountNumbers);
            }
        } else {
            // Handle failed login
            cout << RED << "Invalid username or password. Please try again." << RESET << endl;
            cout << "Press Enter to continue...";
            cin.get();
        }
    }
    
    return 0;
}
