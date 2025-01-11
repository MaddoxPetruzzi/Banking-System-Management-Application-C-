/**
 * Withdraw Operation Implementation
 * 
 * This file implements withdrawal functionality including:
 * - Account validation
 * - Balance verification
 * - Amount validation
 * - Transaction logging
 * - Receipt generation
 */

#include "withdraw.h"
#include "lookUpAccount.h"
#include "utilityFunctions.h"
#include "accountDatabase.h"
#include "userManagement.h"
#include "transactionLog.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <memory>
#include <algorithm>

using namespace std;

/**
 * Process a withdrawal from an account
 * 
 * Process Flow:
 * 1. Locate the account (lookup or direct)
 * 2. Verify sufficient balance
 * 3. Process withdrawal
 * 4. Generate receipt
 * 5. Log transaction
 * 
 * Validation:
 * - Verifies account exists
 * - Checks sufficient balance
 * - Confirms withdrawal amount
 * 
 * @param accountNumber Specific account or -1 for manager lookup
 */
void withdraw(int accountNumber) {
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
    cout << "=== Withdraw from Account ===" << endl;
    cout << "Account Type: " << account->getType() << endl;
    cout << "Account Number: " << account->getAccountNumber() << endl;
    cout << "Account Holder: " << account->getName() << endl;
    cout << "Current Balance: $" << fixed << setprecision(2) << account->getBalance() << endl;
    cout << "==========================" << endl;

    // Get withdrawal amount with validation
    double withdrawalAmount;
    while (true) {
        withdrawalAmount = getValidAmount("Enter withdrawal amount (or 0 to cancel): $");
        if (withdrawalAmount == 0) {
            cout << "Withdrawal cancelled. Returning to previous menu." << endl;
            logTransaction(TransactionType::WITHDRAW, account->getAccountNumber(), 
                         withdrawalAmount, username, TransactionStatus::FAILED, "Cancelled by user");
            return;
        }
        // Check if withdrawal amount is valid
        if (withdrawalAmount <= account->getBalance()) {
            break;
        }
        cout << "Insufficient funds. Available balance: $" 
             << fixed << setprecision(2) << account->getBalance() << endl;
        logTransaction(TransactionType::WITHDRAW, account->getAccountNumber(), 
                      withdrawalAmount, username, TransactionStatus::FAILED, "Insufficient funds");
    }

    // Calculate new balance and confirm
    double newBalance = account->getBalance() - withdrawalAmount;
    cout << "\nWithdrawal Summary:" << endl;
    cout << "Amount to withdraw: $" << fixed << setprecision(2) << withdrawalAmount << endl;
    cout << "New balance will be: $" << fixed << setprecision(2) << newBalance << endl;
    cout << "Confirm withdrawal? (Y/N): ";
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Process withdrawal if confirmed
    if (toupper(confirm) == 'Y') {
        account->setBalance(newBalance);
        if (updateAccountInDatabase(*account)) {
            // Generate receipt
            cout << "\n=== Withdrawal Receipt ===" << endl;
            cout << "Transaction Date: " << getCurrentDate() << endl;
            cout << "Account Type: " << account->getType() << endl;
            cout << "Account Number: " << account->getAccountNumber() << endl;
            cout << "Account Holder: " << account->getName() << endl;
            cout << "Withdrawal Amount: $" << fixed << setprecision(2) << withdrawalAmount << endl;
            cout << "New Balance: $" << fixed << setprecision(2) << newBalance << endl;
            cout << "======================" << endl;
            cout << "Withdrawal successful." << endl;
            
            // Log successful transaction
            string details = isManager ? "Withdrawn by manager" : "Withdrawn by account holder";
            logTransaction(TransactionType::WITHDRAW, account->getAccountNumber(), 
                         withdrawalAmount, username, TransactionStatus::SUCCESS, details);
        } else {
            cout << "Error: Failed to update account in database." << endl;
            logTransaction(TransactionType::WITHDRAW, account->getAccountNumber(), 
                         withdrawalAmount, username, TransactionStatus::FAILED, "Database update failed");
        }
    } else {
        cout << "Withdrawal cancelled." << endl;
        logTransaction(TransactionType::WITHDRAW, account->getAccountNumber(), 
                     withdrawalAmount, username, TransactionStatus::FAILED, "Cancelled by user");
    }
}
