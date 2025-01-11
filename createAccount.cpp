#include "createAccount.h"
#include "accountDatabase.h"
#include "utilityFunctions.h"
#include "userManagement.h"
#include "simpleEncryption.h"
#include "serviceChargeCheckingType.h"
#include "noServiceChargeCheckingType.h"
#include "savingsAccountType.h"
#include "highInterestCheckingType.h"
#include "highInterestSavingsType.h"
#include "certificateOfDepositType.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <limits>
#include <algorithm>

using namespace std;

void createClientAccount();
void createManagerAccount();

void displayMatchingUsers(const vector<User>& users, const vector<unique_ptr<bankAccountType>>& accounts) {
    for (size_t i = 0; i < users.size(); i++) {
        cout << i + 1 << ". " << users[i].username << " (";
        
        // Find all accounts for this user
        cout << "Accounts: ";
        bool firstAccount = true;
        for (const auto& acc : accounts) {
            if (find(users[i].accountNumbers.begin(), users[i].accountNumbers.end(), 
                    acc->getAccountNumber()) != users[i].accountNumbers.end()) {
                if (!firstAccount) {
                    cout << ", ";
                }
                cout << acc->getType() << " #" << acc->getAccountNumber();
                firstAccount = false;
            }
        }
        cout << ")" << endl;
    }
}

void displayUserAccounts(const User& user, const vector<unique_ptr<bankAccountType>>& accounts) {
    cout << "\nCurrent Accounts for " << user.username << ":" << endl;
    cout << string(50, '-') << endl;
    
    for (const auto& acc : accounts) {
        if (find(user.accountNumbers.begin(), user.accountNumbers.end(), 
                acc->getAccountNumber()) != user.accountNumbers.end()) {
            cout << setw(25) << left << acc->getType() 
                 << " #" << acc->getAccountNumber()
                 << "\n   Balance: $" << fixed << setprecision(2) 
                 << acc->getBalance() << endl;
        }
    }
    cout << string(50, '-') << endl;
}

void createAccount() {
    clearScreen();
    cout << "┌─────────────────────────────────┐\n";
    cout << "│         Create Account          │\n";
    cout << "├─────────────────────────────────┤\n";
    cout << "│ 1. Create Client Account        │\n";
    cout << "│ 2. Create Manager Account       │\n";
    cout << "│ 0. Go back                      │\n";
    cout << "└─────────────────────────────────┘\n";
    cout << "Enter your choice: ";

    int choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
        case 1:
            createClientAccount();
            break;
        case 2:
            if (getCurrentUser().role == UserRole::MANAGER) {
                createManagerAccount();
            } else {
                cout << "Error: Only managers can create manager accounts." << endl;
            }
            break;
        case 0:
            return;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void createClientAccount() {
    string name, username;
    char existingUser;
    
    cout << "Create Account for an existing user? (Y/N): ";
    cin >> existingUser;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(existingUser) == 'Y') {
        vector<User> allUsers = getAllUsers();
        vector<unique_ptr<bankAccountType>> allAccounts = loadAccounts();
        vector<User> matchingUsers;
        string searchTerm;

        while (true) {
            matchingUsers.clear();
            cout << "Enter partial name/username to search (0 to cancel): ";
            getline(cin, searchTerm);

            if (searchTerm == "0") {
                return;
            }

            string lowerSearchTerm = toLowerCase(searchTerm);
            for (const auto& user : allUsers) {
                if (user.role == UserRole::CLIENT && 
                    toLowerCase(user.username).find(lowerSearchTerm) != string::npos) {
                    matchingUsers.push_back(user);
                }
            }

            if (matchingUsers.empty()) {
                cout << "No matching users found." << endl;
                continue;
            }

            cout << "\nMatching users:" << endl;
            displayMatchingUsers(matchingUsers, allAccounts);
            
            int userChoice;
            cout << "\nSelect user number (0 to search again): ";
            cin >> userChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (userChoice == 0) {
                continue;
            }

            if (userChoice > 0 && userChoice <= static_cast<int>(matchingUsers.size())) {
                username = matchingUsers[userChoice - 1].username;
                name = matchingUsers[userChoice - 1].username; // Using username as name for consistency
                displayUserAccounts(matchingUsers[userChoice - 1], allAccounts);
                break;
            }

            cout << "Invalid selection." << endl;
        }
    } else {
        cout << "Enter new username: ";
        getline(cin, username);

        if (!isUsernameAvailable(username)) {
            cout << "Username already exists. Please try again." << endl;
            return;
        }

        cout << "Enter account holder's name: ";
        getline(cin, name);
    }

    int accountNumber = getNextAccountNumber();
    double initialBalance;
    int accountType;

    cout << "Enter initial balance: $";
    initialBalance = getValidAmount();

    cout << "\nSelect account type:\n"
         << "1. Service Charge Checking\n"
         << "2. No Service Charge Checking\n"
         << "3. Savings\n"
         << "4. High Interest Checking\n"
         << "5. High Interest Savings\n"
         << "6. Certificate of Deposit\n"
         << "0. Cancel\n"
         << "Enter choice: ";
    accountType = getValidInteger();

    if (accountType == 0) {
        cout << "Account creation cancelled." << endl;
        return;
    }

    if (accountType < 1 || accountType > 6) {
        cout << "Invalid account type." << endl;
        return;
    }

    unique_ptr<bankAccountType> newAccount;
    string accountTypeStr;

    switch(accountType) {
        case 1: {
            double monthlyFee = getValidAmount("Enter monthly fee: $");
            newAccount = make_unique<serviceChargeCheckingType>(name, accountNumber, initialBalance, monthlyFee);
            accountTypeStr = "Service Charge Checking";
            break;
        }
        case 2:
            newAccount = make_unique<noServiceChargeCheckingType>(name, accountNumber, initialBalance);
            accountTypeStr = "No Service Charge Checking";
            break;
        case 3: {
            double interestRate = getValidAmount("Enter interest rate (as decimal): ");
            newAccount = make_unique<savingsAccountType>(name, accountNumber, initialBalance, interestRate);
            accountTypeStr = "Savings";
            break;
        }
        case 4:
            newAccount = make_unique<highInterestCheckingType>(name, accountNumber, initialBalance);
            accountTypeStr = "High Interest Checking";
            break;
        case 5:
            newAccount = make_unique<highInterestSavingsType>(name, accountNumber, initialBalance);
            accountTypeStr = "High Interest Savings";
            break;
        case 6: {
            double interestRate = getValidAmount("Enter interest rate (as decimal): ");
            int maturityMonths = getValidInteger("Enter maturity period (in months): ");
            newAccount = make_unique<certificateOfDepositType>(name, accountNumber, initialBalance, interestRate, maturityMonths);
            accountTypeStr = "Certificate of Deposit";
            break;
        }
    }

    // Show summary and confirm
    cout << "\nAccount Summary:" << endl;
    cout << "=================" << endl;
    cout << "Username: " << username << endl;
    cout << "Account Type: " << accountTypeStr << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Initial Balance: $" << fixed << setprecision(2) << initialBalance << endl;
    cout << "=================" << endl;
    
    cout << "\nConfirm account creation? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(confirm) != 'Y') {
        cout << "Account creation cancelled." << endl;
        return;
    }

    saveAccount(newAccount);

    if (toupper(existingUser) == 'Y') {
        if (addAccountToUser(username, accountNumber)) {
            cout << "Account added successfully to existing user." << endl;
        } else {
            cout << "Error adding account to user." << endl;
        }
    } else {
        User newUser;
        newUser.username = username;
        string defaultPassword = "password";
        newUser.passwordHash = simpleHash(defaultPassword);
        newUser.role = UserRole::CLIENT;
        newUser.accountNumbers.push_back(accountNumber);

        if (addUser(newUser)) {
            cout << "User account created successfully." << endl;
            cout << "Username: " << newUser.username << endl;
            cout << "Default password: " << defaultPassword << endl;
            cout << "Please advise the client to change their password upon first login." << endl;
        } else {
            cout << "Failed to create user account. Please contact system administrator." << endl;
        }
    }
}

void createManagerAccount() {
    string username, password;

    cout << "Enter username for new manager: ";
    getline(cin, username);

    if (!isUsernameAvailable(username)) {
        cout << "Error: Username already exists. Please choose a different username." << endl;
        return;
    }

    cout << "Enter password for new manager: ";
    password = getHiddenInput();

    if (createManagerAccount(username, password)) {
        cout << "Manager account created successfully." << endl;
    } else {
        cout << "Failed to create manager account. Please try again." << endl;
    }
}
