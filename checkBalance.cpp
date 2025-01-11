#include "checkBalance.h"
#include "lookUpAccount.h"
#include "utilityFunctions.h"
#include "accountDatabase.h"
#include "userManagement.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <limits>
#include <algorithm>

using namespace std;

void checkBalance(int accountNumber) {
    unique_ptr<bankAccountType> account;
    
    if (accountNumber == -1) {
        // Manager is checking balance
        account = lookUpAccount();
        if (!account) {
            cout << "Returning to previous menu." << endl;
            return;
        }
    } else {
        // Client is checking their own account balance
        vector<unique_ptr<bankAccountType>> accounts = loadAccounts();
        auto it = std::find_if(accounts.begin(), accounts.end(),
                          [accountNumber](const unique_ptr<bankAccountType>& acc) {
                              return acc->getAccountNumber() == accountNumber;
                          });
        if (it != accounts.end()) {
            account = move(*it);
        } else {
            cout << "Error: Account not found." << endl;
            return;
        }
    }

    try {
        clearScreen();
        cout << "=== Account Balance ===" << endl;
        cout << "Account Number: " << account->getAccountNumber() << endl;
        cout << "Account Holder: " << account->getName() << endl;
        cout << "Account Type: " << account->getType() << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
        cout << "As of: " << getCurrentDate() << endl;
        cout << "=======================" << endl;
    } catch (const exception& e) {
        cerr << "An error occurred while displaying account information: " << e.what() << endl;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
