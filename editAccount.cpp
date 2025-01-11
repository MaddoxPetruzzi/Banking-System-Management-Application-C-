/**
 * Account Edit Implementation
 * 
 * This file implements account modification functionality:
 * - Finding accounts to edit
 * - Modifying account details
 * - Saving changes
 * - Change confirmation
 */

#include "editAccount.h"
#include "lookUpAccount.h"
#include "utilityFunctions.h"
#include "accountDatabase.h"
#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

/**
 * Edit an existing account's information
 * 
 * Process Flow:
 * 1. Search for account to edit
 * 2. Display current information
 * 3. Allow modifications
 * 4. Confirm changes
 * 5. Save updates
 * 
 * Edit Options:
 * - Account holder's name
 * - Account balance
 * - Save/discard changes
 */
void editAccount() {
    // First find the account to edit
    unique_ptr<bankAccountType> account = lookUpAccount();
    if (!account) {
        cout << "No account selected. Returning to main menu." << endl;
        return;
    }

    while (true) {
        // Display edit menu
        clearScreen();
        cout << "Edit Account Menu for Account #" << account->getAccountNumber() << endl;
        cout << "1. Edit Account Holder's Name" << endl;
        cout << "2. Edit Balance" << endl;
        cout << "3. Save Changes and Exit" << endl;
        cout << "4. Cancel and Exit" << endl;
        cout << "Enter your choice: ";

        int choice = getValidInteger();

        // Process user choice
        switch (choice) {
            case 1: {
                // Edit account holder name
                string newName;
                cout << "Current Name: " << account->getName() << endl;
                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, newName);
                account->setName(newName);
                cout << "Name updated successfully." << endl;
                break;
            }
            case 2: {
                // Edit account balance
                double newBalance;
                cout << "Current Balance: $" << fixed << setprecision(2) 
                     << account->getBalance() << endl;
                newBalance = getValidAmount("Enter new balance: $");
                account->setBalance(newBalance);
                cout << "Balance updated successfully." << endl;
                break;
            }
            case 3: {
                // Save changes
                cout << "Are you sure you want to save the following changes?" << endl;
                cout << "Name: " << account->getName() << endl;
                cout << "Balance: $" << fixed << setprecision(2) 
                     << account->getBalance() << endl;
                cout << "Enter 'Y' to confirm or any other key to continue editing: ";
                char confirm;
                cin >> confirm;
                if (toupper(confirm) == 'Y') {
                    if (updateAccountInDatabase(*account)) {
                        cout << "Changes saved successfully." << endl;
                    } else {
                        cout << "Error: Failed to update account in database." << endl;
                    }
                    return;
                }
                break;
            }
            case 4:
                // Cancel without saving
                cout << "Changes discarded. Returning to main menu." << endl;
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
}
