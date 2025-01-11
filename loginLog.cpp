/**
 * Login Logging System Implementation
 * 
 * This file implements login tracking functionality:
 * - Recording login attempts
 * - Storing success/failure status
 * - Displaying login history
 */

#include "loginLog.h"
#include "utilityFunctions.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

const string LOGIN_LOG_FILE = "logins.txt";

/**
 * Log a login attempt to the login log file
 * 
 * Format:
 * datetime,username,status
 * 
 * @param username User attempting to login
 * @param success Whether login was successful
 */
void logLogin(const string& username, bool success) {
    ofstream logFile(LOGIN_LOG_FILE, ios::app);
    if (logFile.is_open()) {
        logFile << getCurrentDate() << ","
                << username << ","
                << (success ? "Success" : "Failed") << endl;
        logFile.close();
    }
}

/**
 * Display login history
 * 
 * Features:
 * - Formatted table display
 * - Date/time of attempts
 * - Success/failure status
 * - Username tracking
 */
void viewLoginHistory() {
    ifstream logFile(LOGIN_LOG_FILE);
    string line;
    
    clearScreen();
    cout << "=== Login History ===" << endl;
    cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;
    
    // Display header
    cout << left 
         << setw(25) << "Date/Time"
         << setw(20) << "Username"
         << "Status" << endl;
    cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;

    // Process each login record
    while (getline(logFile, line)) {
        istringstream iss(line);
        string datetime, username, status;
        
        getline(iss, datetime, ',');
        getline(iss, username, ',');
        getline(iss, status);

        // Display formatted login record
        cout << left
             << setw(25) << datetime
             << setw(20) << username
             << status << endl;
    }

    cout << setfill('=') << setw(60) << "=" << setfill(' ') << endl;
    cout << "\nPress Enter to continue...";
    cin.ignore();
}
