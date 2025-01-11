/**
 * Deposit Operation Implementation
 * 
 * This file implements deposit functionality including:
 * - Account validation
 * - Amount verification
 * - Transaction logging
 * - Receipt generation
 */

#include "deposit.h"
#include "lookUpAccount.h"
#include "utilityFunctions.h"
#include "accountDatabase.h"
#include "userManagement.h"
#include "transactionLog.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

/**
 * Process a deposit to an account
 * 
 * Process Flow:
 * 1. Locate the account (lookup or direct)
 * 2. Verify deposit amount
 * 3. Process deposit
 * 4. Generate receipt
 * 5. Log transaction
 * 
 * @param accountNumber Specific account or -1 for manager lookup
 */
void deposit(int accountNumber) {
    unique_ptr<bankAccountType> account;
    bool isManager = (accountNumber == -1);
    string username = getCurrentUser().username;
    
    // Account identification
    if (accountNumber == -1) {
        // Manager operation - lookup account
        account = lookUpAccount();
        if (!account) {
            cout << "Returning to previous menu." << endl;
            return;
        }
    } else {
        // Client operation - direct account access
        vector<unique_ptr<bankAccountType>> accounts = loadAccounts();
        auto it = find_if(accounts.begin(), accounts.end(),
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

    // Display account information
    clearScreen();
    cout << "=== Deposit to Account ===" << endl;
    cout << "Account Type: " << account->getType() << endl;
    cout << "Account Number: " << account->getAccountNumber() << endl;
    cout << "Account Holder: " << account->getName() << endl;
    cout << "Current Balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
    cout << "=======================" << endl;

    // Get deposit amount
    double depositAmount;
    while (true) {
        depositAmount = getValidAmount("Enter deposit amount (or 0 to cancel): $");
        if (depositAmount == 0) {
            cout << "Deposit cancelled. Returning to previous menu." << endl;
            logTransaction(TransactionType::DEPOSIT, account->getAccountNumber(), 
                         depositAmount, username, TransactionStatus::FAILED, "Cancelled by user");
            return;
        }
        if (depositAmount > 0) {
            break;
        }
        cout << "Invalid amount. Please enter a positive number." << endl;
    }

    // Calculate new balance and confirm
    double newBalance = account->getBalance() + depositAmount;
    cout << "\nDeposit Summary:" << endl;
    cout << "Amount to deposit: $" << fixed << setprecision(2) << depositAmount << endl;
    cout << "New balance will be: $" << fixed << setprecision(2) << newBalance << endl;
    cout << "Confirm deposit? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Process deposit if confirmed
    if (toupper(confirm) == 'Y') {
        account->setBalance(newBalance);
        if (updateAccountInDatabase(*account)) {
            // Generate receipt
            cout << "\n=== Deposit Receipt ===" << endl;
            cout << "Transaction Date: " << getCurrentDate() << endl;
            cout << "Account Type: " << account->getType() << endl;
            cout << "Account Number: " << account->getAccountNumber() << endl;
            cout << "Account Holder: " << account->getName() << endl;
            cout << "Deposit Amount: $" << fixed << setprecision(2) << depositAmount << endl;
            cout << "New Balance: $" << fixed << setprecision(2) << newBalance << endl;
            cout << "====================" << endl;
            cout << "Deposit successful." << endl;
            
            // Log successful transaction
            string details = isManager ? "Deposited by manager" : "Deposited by account holder";
            logTransaction(TransactionType::DEPOSIT, account->getAccountNumber(), 
                         depositAmount, username, TransactionStatus::SUCCESS, details);
        } else {
            cout << "Error: Failed to update account in database." << endl;
            logTransaction(TransactionType::DEPOSIT, account->getAccountNumber(), 
                         depositAmount, username, TransactionStatus::FAILED, "Database update failed");
        }
    } else {
        cout << "Deposit cancelled." << endl;
        logTransaction(TransactionType::DEPOSIT, account->getAccountNumber(), 
                     depositAmount, username, TransactionStatus::FAILED, "Cancelled by user");
    }
}
