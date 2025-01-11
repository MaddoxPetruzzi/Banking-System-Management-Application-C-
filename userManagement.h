#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

/**
 * User Management System
 *
 * Purpose:
 * Manages user authentication, account access, and user data storage for the banking system.
 * Handles both client and manager roles, their permissions, and their account associations.
 *
 * File Structure:
 * - Users stored in users.txt
 * - Data format: username,passwordHash,role,accountNumbers
 * - Account numbers separated by | for multiple accounts
 */

#include <string>
#include <vector>

using namespace std;

// Defines user roles in the system: CLIENT for regular users, MANAGER for administrators
enum class UserRole {
    CLIENT,
    MANAGER
};

// User structure containing all user information including their accounts
struct User {
    string username;
    string passwordHash;
    UserRole role;
    vector<int> accountNumbers;  // List of account numbers owned by the user
};

// Verifies user credentials during login attempt
bool authenticateUser(const string& username, const string& password);

// Saves all user data to the users.txt file
void saveUsers(const vector<User>& users);

// Loads all user data from the users.txt file
vector<User> loadUsers();

// Creates a new user in the system
bool addUser(const User& newUser);

// Returns the currently logged-in user's information
User getCurrentUser();

// Creates the initial admin account if it doesn't exist
void createInitialAdminAccount();

// Changes a user's username
bool changeUsername(const string& oldUsername, const string& newUsername);

// Changes a user's password with verification of old password
bool changePassword(const string& username, const string& oldPassword, const string& newPassword);

// Checks if a username is available for new accounts
bool isUsernameAvailable(const string& username);

// Returns a list of all users in the system
vector<User> getAllUsers();

// Changes a user's password directly (manager function)
bool changePasswordDirectly(const string& username, const string& newPassword);

// Resets the system to only have the admin account
void resetSystemToAdminOnly();

// Creates a new manager account
bool createManagerAccount(const string& username, const string& password);

// Adds an account number to a user's list of accounts
bool addAccountToUser(const string& username, int accountNumber);

// Removes an account number from a user's list of accounts
bool removeAccountFromUser(const string& username, int accountNumber);

// Global variable for the currently logged-in user
extern User currentUser;

#endif // USER_MANAGEMENT_H
