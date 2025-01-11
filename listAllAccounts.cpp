/**
 * Account Listing Implementation
 * 
 * This file implements account listing functionality:
 * - Loading all accounts
 * - Filtering accounts based on access level
 * - Formatting display output
 * - Calculating totals
 */

#include "listAllAccounts.h"
#include "accountDatabase.h"
#include "utilityFunctions.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <limits>
#include <memory>
#include <algorithm>

using namespace std;

/**
 * Display list of accounts
 * 
 * Process Flow:
 * 1. Load all accounts
 * 2. Apply any filters (for client view)
 * 3. Format and display accounts
 * 4. Show summary information
 * 
 * Display Features:
 * - Account numbers
 * - Account types
 * - Current balances
 * - Total balance calculation
 * 
 * @param userAccounts Optional vector of account numbers to filter display
 */
void listAllAccounts(const vector<int>& userAccounts) {
    vector<unique_ptr<bankAccountType>> accounts = loadAccounts();
    vector<unique_ptr<bankAccountType>> filteredAccounts;

    // Filter accounts based on access level
    if (userAccounts.empty()) {
        // Manager view - show all accounts
        filteredAccounts = move(accounts);
    } else {
        // Client view - show only their accounts
        for (auto& account : accounts) {
            if (find(userAccounts.begin(), userAccounts.end(), 
                    account->getAccountNumber()) != userAccounts.end()) {
                filteredAccounts.push_back(move(account));
            }
        }
    }

    // Check if any accounts to display
    if (filteredAccounts.empty()) {
        cout << "No accounts found." << endl;
        return;
    }

    // Display header
    clearScreen();
    cout << "=== Account List ===" << endl;
    cout << setfill('=') << setw(75) << "=" << setfill(' ') << endl;
    cout << setw(10) << left << "Account#" 
         << setw(20) << left << "Name"
         << setw(25) << left << "Type"
         << setw(15) << right << "Balance" << endl;
    cout << setfill('-') << setw(75) << "-" << setfill(' ') << endl;

    // Display accounts and calculate total
    double totalBalance = 0.0;
    for (const auto& account : filteredAccounts) {
        cout << setw(10) << left << account->getAccountNumber()
             << setw(20) << left << account->getName()
             << setw(25) << left << account->getType()
             << setw(15) << right << fixed << setprecision(2) 
             << account->getBalance() << endl;
        totalBalance += account->getBalance();
    }

    // Display summary
    cout << setfill('-') << setw(75) << "-" << setfill(' ') << endl;
    cout << "Total Accounts: " << filteredAccounts.size() << endl;
    cout << "Total Balance: $" << fixed << setprecision(2) << totalBalance << endl;
    cout << setfill('=') << setw(75) << "=" << setfill(' ') << endl;

    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
