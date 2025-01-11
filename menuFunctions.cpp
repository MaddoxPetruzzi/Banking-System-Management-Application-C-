#include "menuFunctions.h"
#include "userManagement.h"
#include "createAccount.h"
#include "editAccount.h"
#include "deposit.h"
#include "withdraw.h"
#include "checkBalance.h"
#include "listAllAccounts.h"
#include "deleteAccount.h"
#include "utilityFunctions.h"
#include "fileLock.h"
#include "transferFunds.h"
#include "accountDatabase.h"
#include "transactionLog.h"
#include "loginLog.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

// Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

using namespace std;

const string FILE_NAME = "accounts.txt";

void displayLoginMenu(string& username, string& password) {
    clearScreen();
    cout << CYAN;
    cout << "╔═══════════════════════════════╗\n";
    cout << "║        BankSystem Login       ║\n";
    cout << "╠═══════════════════════════════╣\n";
    cout << "║                               ║\n";
    cout << "║  Enter username or 'exit':    ║\n";
    cout << "║                               ║\n";
    cout << "╚═══════════════════════════════╝\n" << RESET;
    cout << "Username: ";
    getline(cin, username);
    
    if (username != "exit") {
        cout << "Password: ";
        password = getHiddenInput();
    }
}

void changeUsernameMenu() {
    clearScreen();
    cout << "┌─────────────────────────────────┐\n";
    cout << "│       Change Username Menu      │\n";
    cout << "└─────────────────────────────────┘\n";
    
    string newUsername;
    cout << "Enter new username: ";
    getline(cin, newUsername);
    
    if (changeUsername(getCurrentUser().username, newUsername)) {
        cout << GREEN << "Username changed successfully." << RESET << endl;
    } else {
        cout << RED << "Failed to change username." << RESET << endl;
    }
}

void changePasswordMenu(bool isManager) {
    clearScreen();
    cout << "┌─────────────────────────────────┐\n";
    cout << "│       Change Password Menu      │\n";
    cout << "└─────────────────────────────────┘\n";

    string username;
    if (isManager) {
        vector<User> users = getAllUsers();
        cout << "List of users:\n";
        for (size_t i = 0; i < users.size(); ++i) {
            cout << i + 1 << ". " << users[i].username;
            if (!users[i].accountNumbers.empty()) {
                cout << " (Account #: " << users[i].accountNumbers[0] << ")";
            }
            cout << "\n";
        }

        int choice;
        cout << "Enter the number of the user to change password (0 to cancel): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0 || choice > static_cast<int>(users.size())) {
            cout << "Operation cancelled or invalid choice.\n";
            return;
        }

        username = users[choice - 1].username;
    } else {
        username = getCurrentUser().username;
    }

    string oldPassword, newPassword, confirmPassword;

    cout << "Changing password for user: " << username << endl;
    if (!isManager) {
        cout << "Enter current password: ";
        oldPassword = getHiddenInput();
    }
    cout << "Enter new password: ";
    newPassword = getHiddenInput();
    cout << "Confirm new password: ";
    confirmPassword = getHiddenInput();

    if (newPassword != confirmPassword) {
        cout << RED << "Error: Passwords do not match. Password change cancelled." << RESET << endl;
        cout << "Press Enter to continue...";
        cin.get();
        return;
    }

    cout << "Confirm changing password for '" << username << "' (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (tolower(confirm) == 'y') {
        bool success;
        if (isManager) {
            success = changePasswordDirectly(username, newPassword);
        } else {
            success = changePassword(username, oldPassword, newPassword);
        }

        if (success) {
            cout << GREEN << "Password changed successfully." << RESET << endl;
        } else {
            cout << RED << "Failed to change password. ";
            if (!isManager) {
                cout << "Please make sure you entered the correct current password.";
            }
            cout << RESET << endl;
        }
    } else {
        cout << "Password change cancelled.\n";
    }

    cout << "Press Enter to continue...";
    cin.get();
}

unique_ptr<bankAccountType> selectAccount(const vector<int>& accountNumbers, const string& prompt) {
    vector<unique_ptr<bankAccountType>> accounts = loadAccounts();
    vector<unique_ptr<bankAccountType>> userAccounts;
    
    for (auto& account : accounts) {
        if (find(accountNumbers.begin(), accountNumbers.end(), 
                account->getAccountNumber()) != accountNumbers.end()) {
            userAccounts.push_back(move(account));
        }
    }

    if (userAccounts.empty()) {
        cout << "No accounts found." << endl;
        return nullptr;
    }

    cout << "\n" << prompt << endl;
    cout << string(50, '-') << endl;
    for (size_t i = 0; i < userAccounts.size(); i++) {
        cout << i + 1 << ". " << setw(25) << left << userAccounts[i]->getType() 
             << " #" << userAccounts[i]->getAccountNumber() 
             << "\n   Balance: $" << fixed << setprecision(2) 
             << userAccounts[i]->getBalance() << endl;
    }
    cout << "0. Cancel" << endl;
    cout << string(50, '-') << endl;

    int choice;
    cout << "Select account (0 to cancel): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == 0 || choice > static_cast<int>(userAccounts.size())) {
        return nullptr;
    }

    return move(userAccounts[choice - 1]);
}
void displayClientMenu(const vector<int>& accountNumbers) {
    char choice;
    int lockFd = -1;

    do {
        clearScreen();
        cout << "┌─────────────────────────────────┐\n";
        cout << "│           Client Menu           │\n";
        cout << "│      User: " << setw(16) << left << getCurrentUser().username << "     │\n";
        cout << "├─────────────────────────────────┤\n";
        cout << "│ " << GREEN << "1" << RESET << ". Deposit                      │\n";
        cout << "│ " << GREEN << "2" << RESET << ". Withdraw                     │\n";
        cout << "│ " << GREEN << "3" << RESET << ". Check Balance                │\n";
        cout << "│ " << GREEN << "4" << RESET << ". Transfer Funds               │\n";
        cout << "│ " << GREEN << "5" << RESET << ". List My Accounts             │\n";
        cout << "│ " << GREEN << "6" << RESET << ". View My Transactions         │\n";
        cout << "│ " << GREEN << "7" << RESET << ". Change Password              │\n";
        cout << "│ " << RED << "0" << RESET << ". Logout                       │\n";
        cout << "└─────────────────────────────────┘\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (accountNumbers.empty() && (choice >= '1' && choice <= '6')) {
            cout << "You don't have any accounts yet. Please contact a manager to create an account." << endl;
            cout << "Press Enter to continue...";
            cin.get();
            continue;
        }

        switch (choice) {
            case '1':
            case '2':
            case '3': {
                auto selectedAccount = selectAccount(accountNumbers, 
                    "Select account for operation:");
                if (selectedAccount) {
                    lockFd = acquireLock(FILE_NAME);
                    if (lockFd == -1) {
                        cout << "Someone is currently editing the file. Please try again later." << endl;
                        break;
                    }
                    switch (choice) {
                        case '1': deposit(selectedAccount->getAccountNumber()); break;
                        case '2': withdraw(selectedAccount->getAccountNumber()); break;
                        case '3': checkBalance(selectedAccount->getAccountNumber()); break;
                    }
                    releaseLock(lockFd);
                }
                break;
            }
            case '4': {
                if (accountNumbers.size() < 2) {
                    cout << "You need at least two accounts to perform transfers." << endl;
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                lockFd = acquireLock(FILE_NAME);
                if (lockFd == -1) {
                    cout << "Someone is currently editing the file. Please try again later." << endl;
                    break;
                }
                transferFunds(getCurrentUser().accountNumbers[0]);
                releaseLock(lockFd);
                break;
            }
            case '5':
                listAllAccounts(accountNumbers);
                break;
            case '6':
                viewUserTransactionHistory(getCurrentUser().username);
                break;
            case '7':
                changePasswordMenu(false);
                break;
            case '0':
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

        if (choice != '0' && choice != '7') {
            cout << "Press Enter to continue...";
            cin.get();
        }
    } while (choice != '0');
}

void displayManagerMenu() {
    char choice;
    int lockFd = -1;

    do {
        clearScreen();
        cout << "┌─────────────────────────────────┐\n";
        cout << "│         Bank Manager Menu       │\n";
        cout << "│      User: " << setw(16) << left << getCurrentUser().username << "     │\n";
        cout << "├─────────────────────────────────┤\n";
        cout << "│ " << CYAN << "1" << RESET << ". Create Account               │\n";
        cout << "│ " << CYAN << "2" << RESET << ". Edit Account                 │\n";
        cout << "│ " << CYAN << "3" << RESET << ". Deposit                      │\n";
        cout << "│ " << CYAN << "4" << RESET << ". Withdraw                     │\n";
        cout << "│ " << CYAN << "5" << RESET << ". Check Balance                │\n";
        cout << "│ " << CYAN << "6" << RESET << ". List All Accounts            │\n";
        cout << "│ " << CYAN << "7" << RESET << ". Delete Account               │\n";
        cout << "│ " << CYAN << "8" << RESET << ". Transfer Funds               │\n";
        cout << "│ " << CYAN << "9" << RESET << ". View Transaction History     │\n";
        cout << "│ " << CYAN << "L" << RESET << ". View Login History           │\n";
        cout << "│ " << CYAN << "U" << RESET << ". Change Username              │\n";
        cout << "│ " << CYAN << "P" << RESET << ". Change Password              │\n";
        cout << "│ " << MAGENTA << "R" << RESET << ". Reset System to Admin Only   │\n";
        cout << "│ " << RED << "0" << RESET << ". Logout                       │\n";
        cout << "└─────────────────────────────────┘\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        vector<unique_ptr<bankAccountType>> accounts = loadAccounts();
        vector<User> users = getAllUsers();
        int numClients = count_if(users.begin(), users.end(), 
                                [](const User& user) { return user.role == UserRole::CLIENT; });

        switch (toupper(choice)) {
            case '1':
                lockFd = acquireLock(FILE_NAME);
                if (lockFd == -1) {
                    cout << "Someone is currently editing the file. Please try again later." << endl;
                    break;
                }
                createAccount();
                releaseLock(lockFd);
                break;
            case '2':
            case '3':
            case '4':
            case '5':
            case '7':
            case '8': {
                if (accounts.empty()) {
                    cout << "No accounts exist in the system. Create an account first." << endl;
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                lockFd = acquireLock(FILE_NAME);
                if (lockFd == -1) {
                    cout << "Someone is currently editing the file. Please try again later." << endl;
                    break;
                }
                switch (toupper(choice)) {
                    case '2': editAccount(); break;
                    case '3': deposit(); break;
                    case '4': withdraw(); break;
                    case '5': checkBalance(); break;
                    case '7': deleteAccount(); break;
                    case '8': {
                        if (numClients == 0) {
                            cout << "No clients exist in the system. Create a client account first." << endl;
                            cout << "Press Enter to continue...";
                            cin.get();
                            break;
                        }
                        transferFunds();
                        break;
                    }
                }
                releaseLock(lockFd);
                break;
            }
            case '6': {
                if (accounts.empty()) {
                    cout << "No accounts exist in the system." << endl;
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                listAllAccounts();
                break;
            }
            case '9':
                viewTransactionHistory(-1);  // View all transactions
                break;
            case 'L':
                viewLoginHistory();
                break;
            case 'U':
            case 'P': {
                if (numClients == 0) {
                    cout << "No clients exist in the system to modify." << endl;
                    cout << "Press Enter to continue...";
                    cin.get();
                    break;
                }
                if (toupper(choice) == 'U') {
                    changeUsernameMenu();
                } else {
                    changePasswordMenu(true);
                }
                break;
            }
            case 'R': {
                cout << "Are you sure you want to reset the system? This will remove all users except admin. (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (tolower(confirm) == 'y') {
                    resetSystemToAdminOnly();
                    cout << "System has been reset. You will be logged out now." << endl;
                    cout << "Press Enter to continue...";
                    cin.get();
                    return;
                }
                break;
            }
            case '0':
                cout << "Logging out...\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

        if (choice != '0' && toupper(choice) != 'P' && toupper(choice) != 'R') {
            cout << "Press Enter to continue...";
            cin.get();
        }
    } while (choice != '0');
}
