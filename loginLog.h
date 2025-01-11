/**
 * Login Logging System Header
 * 
 * Purpose:
 * Provides functionality for tracking and displaying login attempts.
 * Helps monitor system access and security.
 * 
 * Features:
 * - Login attempt recording
 * - Success/failure tracking
 * - Login history viewing
 */

#ifndef LOGIN_LOG_H
#define LOGIN_LOG_H

#include <string>

using namespace std;

// Records a login attempt to the log file
void logLogin(const string& username, bool success);

// Displays the login history
void viewLoginHistory();

#endif // LOGIN_LOG_H
