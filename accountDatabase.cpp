/**
 * Account Database Implementation
 * 
 * This file implements all account database operations including:
 * - Account creation and storage
 * - Account retrieval and loading
 * - Account updates and deletions
 * - File-based persistence with encryption
 */

#include "accountDatabase.h"
#include "simpleEncryption.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <algorithm>
#include "serviceChargeCheckingType.h"
#include "noServiceChargeCheckingType.h"
#include "savingsAccountType.h"
#include "highInterestCheckingType.h"
#include "highInterestSavingsType.h"
#include "certificateOfDepositType.h"

using namespace std;

const string FILE_NAME = "accounts.txt";
const string ENCRYPTION_KEY = "your_secret_key_here";

/**
 * Saves an account to the encrypted database file
 * 
 * Process:
 * 1. Decrypts existing file content
 * 2. Appends new account information
 * 3. Re-encrypts the entire file
 * 
 * File Format:
 * accountNumber,name,type,balance
 * 
 * Error Handling:
 * - Checks for file open/write permissions
 * - Reports errors to cerr stream
 * 
 * @param account The account to save
 */
void saveAccount(const unique_ptr<bankAccountType>& account) {
    // Decrypt existing content
    string content;
    content = decryptFile(FILE_NAME, simpleHash(ENCRYPTION_KEY));

    // Format new account data
    stringstream ss;
    ss << content;
    ss << account->getAccountNumber() << ","
       << account->getName() << ","
       << account->getType() << ","
       << account->getBalance() << "\n";

    content = ss.str();

    // Write to temporary file before encryption
    ofstream tempFile("temp_accounts.txt");
    if (tempFile.is_open()) {
        tempFile << content;
        tempFile.close();
    } else {
        cerr << "Error: Unable to write to temp_accounts.txt" << endl;
    }

    // Encrypt the final file
    encryptFile(FILE_NAME, simpleHash(ENCRYPTION_KEY));
}

/**
 * Loads all accounts from the encrypted database file
 * 
 * Process:
 * 1. Decrypts the file
 * 2. Reads each line representing an account
 * 3. Creates appropriate account objects based on type
 * 4. Initializes account properties
 * 
 * Account Types Handled:
 * - Service Charge Checking
 * - No Service Charge Checking
 * - Savings
 * - High Interest Checking
 * - High Interest Savings
 * - Certificate of Deposit
 * 
 * @return vector<unique_ptr<bankAccountType>> List of all accounts
 */
vector<unique_ptr<bankAccountType>> loadAccounts() {
    vector<unique_ptr<bankAccountType>> accounts;

    // Decrypt file content
    string content = decryptFile(FILE_NAME, simpleHash(ENCRYPTION_KEY));

    // Process each line
    istringstream file(content);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string accountNumber, name, type, balance;

        // Parse comma-separated values
        if (getline(iss, accountNumber, ',') &&
            getline(iss, name, ',') &&
            getline(iss, type, ',') &&
            getline(iss, balance)) {

            int accNum = stoi(accountNumber);
            double bal = stod(balance);

            // Create appropriate account type
            unique_ptr<bankAccountType> account;

            // Determine account type and create corresponding object
            if (type.find("ServiceChargeChecking") != string::npos) {
                account = make_unique<serviceChargeCheckingType>(name, accNum, bal);
            } else if (type.find("NoServiceChargeChecking") != string::npos) {
                account = make_unique<noServiceChargeCheckingType>(name, accNum, bal);
            } else if (type.find("Savings") != string::npos) {
                account = make_unique<savingsAccountType>(name, accNum, bal, 0.0);
            } else if (type.find("HighInterestChecking") != string::npos) {
                account = make_unique<highInterestCheckingType>(name, accNum, bal);
            } else if (type.find("HighInterestSavings") != string::npos) {
                account = make_unique<highInterestSavingsType>(name, accNum, bal);
            } else if (type.find("CertificateOfDeposit") != string::npos) {
                account = make_unique<certificateOfDepositType>(name, accNum, bal, 0.0, 0);
            } else {
                // Default to basic account type if type unknown
                account = make_unique<noServiceChargeCheckingType>(name, accNum, bal);
            }

            if (account) {
                accounts.push_back(move(account));
            }
        }
    }

    return accounts;
}

/**
 * Generates the next available account number
 * 
 * Logic:
 * 1. Loads all existing accounts
 * 2. If no accounts exist, starts at 1000
 * 3. Otherwise, uses highest existing number + 1
 * 
 * @return int The next available account number
 */
int getNextAccountNumber() {
    vector<unique_ptr<bankAccountType>> accounts = loadAccounts();
    if (accounts.empty()) {
        return 1000; // Start with account number 1000 if no accounts exist
    }
    return accounts.back()->getAccountNumber() + 1;
}

/**
 * Removes an account from the database
 * 
 * Process:
 * 1. Loads all accounts
 * 2. Finds and removes specified account
 * 3. Rewrites remaining accounts to file
 * 
 * Error Handling:
 * - Verifies account exists before removal
 * - Ensures file operations complete successfully
 * 
 * @param accountNumber The account number to remove
 * @return bool True if account was found and removed
 */
bool removeAccountFromDatabase(int accountNumber) {
    vector<unique_ptr<bankAccountType>> accounts = loadAccounts();
    auto it = remove_if(accounts.begin(), accounts.end(),
                        [accountNumber](const unique_ptr<bankAccountType>& account) {
                            return account->getAccountNumber() == accountNumber;
                        });

    if (it != accounts.end()) {
        accounts.erase(it, accounts.end());

        // Rebuild file content
        string content;
        for (const auto& account : accounts) {
            content += to_string(account->getAccountNumber()) + ","
                    + account->getName() + ","
                    + account->getType() + ","
                    + to_string(account->getBalance()) + "\n";
        }

        ofstream tempFile("temp_accounts.txt");
        if (tempFile.is_open()) {
            tempFile << content;
            tempFile.close();
        } else {
            cerr << "Error: Unable to write to temp_accounts.txt" << endl;
            return false;
        }

        encryptFile(FILE_NAME, simpleHash(ENCRYPTION_KEY));
        return true;
    }
    return false;
}

/**
 * Updates an existing account in the database
 * 
 * Process:
 * 1. Loads all accounts
 * 2. Finds matching account
 * 3. Updates account information
 * 4. Saves all accounts back to file
 * 
 * Update Fields:
 * - Account holder name
 * - Account balance
 * - Account type specific data
 * 
 * @param updatedAccount The account with updated information
 * @return bool True if account was found and updated
 */
bool updateAccountInDatabase(const bankAccountType& updatedAccount) {
    vector<unique_ptr<bankAccountType>> accounts = loadAccounts();
    bool found = false;

    for (auto& account : accounts) {
        if (account->getAccountNumber() == updatedAccount.getAccountNumber()) {
            account->setName(updatedAccount.getName());
            account->setBalance(updatedAccount.getBalance());
            found = true;
            break;
        }
    }

    if (found) {
        string content;
        for (const auto& account : accounts) {
            content += to_string(account->getAccountNumber()) + ","
                    + account->getName() + ","
                    + account->getType() + ","
                    + to_string(account->getBalance()) + "\n";
        }

        ofstream tempFile("temp_accounts.txt");
        if (tempFile.is_open()) {
            tempFile << content;
            tempFile.close();
        } else {
            cerr << "Error: Unable to write to temp_accounts.txt" << endl;
            return false;
        }

        encryptFile(FILE_NAME, simpleHash(ENCRYPTION_KEY));
        return true;
    }
    return false;
}
