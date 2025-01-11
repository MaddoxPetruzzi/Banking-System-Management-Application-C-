/**
 * User Management Implementation
 * 
 * Handles all user-related operations including:
 * - User authentication and login
 * - User data storage and retrieval
 * - Password management
 * - Account associations
 * - Role-based access control
 */

#include "userManagement.h"
#include "simpleEncryption.h"
#include "loginLog.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

using namespace std;

// File paths and encryption keys for user data storage
const string USER_FILE = "users.txt";
const string USER_ENCRYPTION_KEY = "user_file_encryption_key";

// Current user's session information
User currentUser;

// Authenticates user login attempts and maintains login history
bool authenticateUser(const string& username, const string& password) {
    vector<User> users = loadUsers();
    for (const auto& user : users) {
        if (user.username == username && user.passwordHash == simpleHash(password)) {
            currentUser = user;
            logLogin(username, true);  // Log successful login
            return true;
        }
    }
    logLogin(username, false);  // Log failed login attempt
    return false;
}

// Saves all user data to the users.txt file in the specified format
void saveUsers(const vector<User>& users) {
    stringstream ss;
    for (const auto& user : users) {
        ss << user.username << "," << user.passwordHash << ","
           << (user.role == UserRole::MANAGER ? "MANAGER" : "CLIENT");
        
        if (user.role == UserRole::MANAGER) {
            ss << ",-1";  // Managers have -1 as account number
        } else {
            // Save all account numbers with | separator for multiple accounts
            for (size_t i = 0; i < user.accountNumbers.size(); ++i) {
                ss << "," << user.accountNumbers[i];
                if (i < user.accountNumbers.size() - 1) {
                    ss << "|";
                }
            }
        }
        ss << "\n";
    }
    
    string content = ss.str();
    ofstream outFile(USER_FILE);
    if (outFile.is_open()) {
        outFile << content;
        outFile.close();
    } else {
        cerr << "Error: Unable to write to " << USER_FILE << endl;
    }
}

// Loads and parses user data from the users.txt file
vector<User> loadUsers() {
    vector<User> users;
    ifstream inFile(USER_FILE);
    string line;
    
    while (getline(inFile, line)) {
        istringstream lineStream(line);
        string username, passwordHash, roleStr;
        
        if (getline(lineStream, username, ',') &&
            getline(lineStream, passwordHash, ',') &&
            getline(lineStream, roleStr, ',')) {
            
            User user;
            user.username = username;
            user.passwordHash = passwordHash;
            user.role = (roleStr == "MANAGER" ? UserRole::MANAGER : UserRole::CLIENT);
            
            // Parse account numbers, handling both single and multiple accounts
            string accountsStr;
            while (getline(lineStream, accountsStr, ',')) {
                try {
                    if (!accountsStr.empty()) {
                        istringstream accountStream(accountsStr);
                        string accountNum;
                        while (getline(accountStream, accountNum, '|')) {
                            if (!accountNum.empty()) {
                                int accNum = stoi(accountNum);
                                user.accountNumbers.push_back(accNum);
                            }
                        }
                    }
                } catch (const exception& e) {
                    cerr << "Error parsing account number: " << accountsStr << endl;
                    continue;
                }
            }
            
            // Initialize manager accounts with -1 if no accounts specified
            if (user.role == UserRole::MANAGER && user.accountNumbers.empty()) {
                user.accountNumbers.push_back(-1);
            }
            
            users.push_back(user);
        }
    }
    
    return users;
}
// Adds a new user to the system after checking for duplicates
bool addUser(const User& newUser) {
    vector<User> users = loadUsers();
    for (const auto& user : users) {
        if (user.username == newUser.username) {
            return false; // User already exists
        }
    }
    users.push_back(newUser);
    saveUsers(users);
    return true;
}

// Returns current logged-in user's information
User getCurrentUser() {
    return currentUser;
}

// Creates the initial admin account if it doesn't exist in the system
void createInitialAdminAccount() {
    vector<User> users = loadUsers();
    bool adminExists = false;

    for (const auto& user : users) {
        if (user.username == "admin" && user.role == UserRole::MANAGER) {
            adminExists = true;
            return;
        }
    }

    if (!adminExists) {
        User adminUser;
        adminUser.username = "admin";
        adminUser.passwordHash = simpleHash("admin");
        adminUser.role = UserRole::MANAGER;
        adminUser.accountNumbers.push_back(-1);
        
        users.push_back(adminUser);
        saveUsers(users);
    }
}

// Changes a user's username and updates all references
bool changeUsername(const string& oldUsername, const string& newUsername) {
    vector<User> users = loadUsers();
    auto it = find_if(users.begin(), users.end(),
                      [&oldUsername](const User& user) { 
                          return user.username == oldUsername; 
                      });
    
    if (it != users.end()) {
        it->username = newUsername;
        saveUsers(users);
        if (currentUser.username == oldUsername) {
            currentUser.username = newUsername;
        }
        return true;
    }
    return false;
}

// Changes a user's password after verifying their old password
bool changePassword(const string& username, const string& oldPassword, const string& newPassword) {
    vector<User> users = loadUsers();
    auto it = find_if(users.begin(), users.end(),
                      [&username](const User& user) { 
                          return user.username == username; 
                      });
    
    if (it != users.end() && it->passwordHash == simpleHash(oldPassword)) {
        it->passwordHash = simpleHash(newPassword);
        saveUsers(users);
        if (currentUser.username == username) {
            currentUser.passwordHash = simpleHash(newPassword);
        }
        return true;
    }
    return false;
}

// Checks if a username is available for new account creation
bool isUsernameAvailable(const string& username) {
    vector<User> users = loadUsers();
    return find_if(users.begin(), users.end(),
                   [&username](const User& user) { 
                       return user.username == username; 
                   }) == users.end();
}

// Returns list of all users in the system
vector<User> getAllUsers() {
    return loadUsers();
}

// Changes a user's password directly (manager function, no old password required)
bool changePasswordDirectly(const string& username, const string& newPassword) {
    vector<User> users = loadUsers();
    auto it = find_if(users.begin(), users.end(),
                      [&username](const User& user) { 
                          return user.username == username; 
                      });
    
    if (it != users.end()) {
        it->passwordHash = simpleHash(newPassword);
        saveUsers(users);
        if (currentUser.username == username) {
            currentUser.passwordHash = simpleHash(newPassword);
        }
        return true;
    }
    return false;
}

// Adds a new account number to a user's list of accounts
bool addAccountToUser(const string& username, int accountNumber) {
    vector<User> users = loadUsers();
    auto it = find_if(users.begin(), users.end(),
                      [&username](const User& user) { 
                          return user.username == username; 
                      });
    
    if (it != users.end() && it->role == UserRole::CLIENT) {
        it->accountNumbers.push_back(accountNumber);
        saveUsers(users);
        if (currentUser.username == username) {
            currentUser.accountNumbers.push_back(accountNumber);
        }
        return true;
    }
    return false;
}

// Removes an account number from a user's list of accounts
bool removeAccountFromUser(const string& username, int accountNumber) {
    vector<User> users = loadUsers();
    auto it = find_if(users.begin(), users.end(),
                      [&username](const User& user) { 
                          return user.username == username; 
                      });

    if (it != users.end() && it->role == UserRole::CLIENT) {
        auto& accounts = it->accountNumbers;
        auto accountIt = find(accounts.begin(), accounts.end(), accountNumber);

        if (accountIt != accounts.end()) {
            accounts.erase(accountIt);
            saveUsers(users);

            if (currentUser.username == username) {
                auto& currentAccounts = currentUser.accountNumbers;
                currentAccounts.erase(
                    find(currentAccounts.begin(), currentAccounts.end(), accountNumber)
                );
            }
            return true;
        }
    }
    return false;
}

// Resets the system to only have the admin account
void resetSystemToAdminOnly() {
    User adminUser;
    adminUser.username = "admin";
    adminUser.passwordHash = simpleHash("admin");
    adminUser.role = UserRole::MANAGER;
    adminUser.accountNumbers.push_back(-1);
    
    vector<User> users = {adminUser};
    saveUsers(users);
}

// Creates a new manager account with appropriate permissions
bool createManagerAccount(const string& username, const string& password) {
    User newManager;
    newManager.username = username;
    newManager.passwordHash = simpleHash(password);
    newManager.role = UserRole::MANAGER;
    newManager.accountNumbers.push_back(-1);
    
    return addUser(newManager);
}
