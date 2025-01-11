/**
 * Account Deletion Implementation
 * 
 * This file implements account deletion functionality:
 * - Finding accounts to delete
 * - Confirming deletion
 * - Removing accounts from system
 * - Multiple deletion handling
 */

#include "deleteAccount.h"
#include "lookUpAccount.h"
#include "accountDatabase.h"
#include "utilityFunctions.h"
#include <iostream>
#include <iomanip>
#include <memory>
#include <limits>

using namespace std;

/**
 * Delete an account from the system
 * 
 * Process Flow:
 * 1. Search for account to delete
 * 2. Display account details
 * 3. Confirm deletion
 * 4. Remove account
 * 5. Option to delete another
 * 
 * Safety Features:
 * - Displays full account info before deletion
 * - Requires confirmation
 * - Cannot be undone warning
 */
void deleteAccount() {
    while (true) {
        // Find account to delete
        unique_ptr<bankAccountType> account = lookUpAccount();
        if (!account) {
            cout << "Returning to previous menu." << endl;
            return;
        }

        // Display account details for verification
        cout << "\nAccount Details:" << endl;
        cout << "Account Number: " << account->getAccountNumber() << endl;
        cout << "Account Holder: " << account->getName() << endl;
        cout << "Account Type: " << account->getType() << endl;
        cout << "Current Balance: $" << fixed << setprecision(2) 
             << account->getBalance() << endl;

        // Get deletion confirmation
        cout << "\nAre you sure you want to delete this account? "
             << "This action cannot be undone. (Y/N): ";
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Process deletion if confirmed
        if (toupper(confirm) == 'Y') {
            if (removeAccountFromDatabase(account->getAccountNumber())) {
                cout << "Account successfully deleted." << endl;
            } else {
                cout << "Error: Failed to delete the account." << endl;
            }
        } else {
            cout << "Account deletion cancelled." << endl;
        }

        // Check if user wants to delete another account
        cout << "\nWould you like to delete another account? (Y/N): ";
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (toupper(confirm) != 'Y') {
            cout << "Returning to main menu." << endl;
            return;
        }
    }
}
