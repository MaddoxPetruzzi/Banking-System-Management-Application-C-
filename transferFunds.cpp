#include "transferFunds.h"
#include "accountDatabase.h"
#include "userManagement.h"
#include "utilityFunctions.h"
#include "menuFunctions.h"
#include "transactionLog.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

void transferFunds(int clientAccountNumber) {
    vector<int> userAccounts;
    string username;

    if (clientAccountNumber == -1) {
        vector<User> allUsers = getAllUsers();
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
            for (size_t i = 0; i < matchingUsers.size(); i++) {
                cout << i + 1 << ". " << matchingUsers[i].username << endl;
            }
            
            int userChoice;
            cout << "\nSelect user number (0 to search again): ";
            cin >> userChoice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (userChoice == 0) {
                continue;
            }

            if (userChoice > 0 && userChoice <= static_cast<int>(matchingUsers.size())) {
                userAccounts = matchingUsers[userChoice - 1].accountNumbers;
                username = matchingUsers[userChoice - 1].username;
                break;
            }

            cout << "Invalid selection." << endl;
        }
    } else {
        User currentUser = getCurrentUser();
        userAccounts = currentUser.accountNumbers;
        username = currentUser.username;
    }

    if (userAccounts.size() < 2) {
        cout << "Need at least two accounts to transfer funds." << endl;
        return;
    }

    auto sourceAccount = selectAccount(userAccounts, "Select source account:");
    if (!sourceAccount) {
        return;
    }

    vector<int> destAccountNumbers;
    for (int accNum : userAccounts) {
        if (accNum != sourceAccount->getAccountNumber()) {
            destAccountNumbers.push_back(accNum);
        }
    }

    auto destAccount = selectAccount(destAccountNumbers, "Select destination account:");
    if (!destAccount) {
        return;
    }

    double transferAmount;
    while (true) {
        transferAmount = getValidAmount("Enter transfer amount (0 to cancel): $");
        if (transferAmount == 0) {
            cout << "Transfer cancelled." << endl;
            logTransaction(TransactionType::TRANSFER, 
                         sourceAccount->getAccountNumber(),
                         transferAmount, 
                         username, 
                         TransactionStatus::FAILED,
                         "Transfer cancelled by user");
            return;
        }
        if (transferAmount <= sourceAccount->getBalance()) {
            break;
        }
        cout << "Insufficient funds. Available balance: $" 
             << fixed << setprecision(2) << sourceAccount->getBalance() << endl;
        logTransaction(TransactionType::TRANSFER, 
                      sourceAccount->getAccountNumber(),
                      transferAmount, 
                      username, 
                      TransactionStatus::FAILED,
                      "Insufficient funds");
    }

    cout << "\nTransfer Summary:" << endl;
    cout << "From: " << sourceAccount->getType() << " #" << sourceAccount->getAccountNumber() << endl;
    cout << "To: " << destAccount->getType() << " #" << destAccount->getAccountNumber() << endl;
    cout << "Amount: $" << fixed << setprecision(2) << transferAmount << endl;
    cout << "\nConfirm transfer? (Y/N): ";
    
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (toupper(confirm) != 'Y') {
        cout << "Transfer cancelled." << endl;
        logTransaction(TransactionType::TRANSFER, 
                      sourceAccount->getAccountNumber(),
                      transferAmount, 
                      username, 
                      TransactionStatus::FAILED,
                      "Transfer cancelled by user");
        return;
    }

    sourceAccount->withdraw(transferAmount);
    destAccount->deposit(transferAmount);

    if (updateAccountInDatabase(*sourceAccount) && 
        updateAccountInDatabase(*destAccount)) {
        cout << "\n=== Transfer Receipt ===" << endl;
        cout << "Date: " << getCurrentDate() << endl;
        cout << "From Account: " << sourceAccount->getType() 
             << " #" << sourceAccount->getAccountNumber() << endl;
        cout << "To Account: " << destAccount->getType() 
             << " #" << destAccount->getAccountNumber() << endl;
        cout << "Amount: $" << fixed << setprecision(2) << transferAmount << endl;
        cout << "\nNew Balances:" << endl;
        cout << "Source Account: $" << fixed << setprecision(2) << sourceAccount->getBalance() << endl;
        cout << "Destination Account: $" << fixed << setprecision(2) << destAccount->getBalance() << endl;
        cout << "=====================" << endl;
        cout << "Transfer completed successfully." << endl;

        // Log successful transfer
        string transferDetails = "Transfer between accounts";
        logTransaction(TransactionType::TRANSFER, 
                      sourceAccount->getAccountNumber(),
                      transferAmount, 
                      username, 
                      TransactionStatus::SUCCESS,
                      transferDetails,
                      destAccount->getAccountNumber());
    } else {
        cout << "Error: Failed to update accounts in database." << endl;
        logTransaction(TransactionType::TRANSFER, 
                      sourceAccount->getAccountNumber(),
                      transferAmount, 
                      username, 
                      TransactionStatus::FAILED,
                      "Database update failed",
                      destAccount->getAccountNumber());
    }
}
