/**
 * Transaction Logging Implementation
 * 
 * This file implements transaction logging functionality:
 * - Recording all account transactions
 * - Viewing transaction history
 * - Formatting transaction records
 */

#include "transactionLog.h"
#include "utilityFunctions.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

const string TRANSACTION_LOG_FILE = "transactions.txt";

/**
 * Convert transaction type to string representation
 * 
 * @param type The transaction type enum
 * @return string String representation of transaction type
 */
string transactionTypeToString(TransactionType type) {
    switch (type) {
        case TransactionType::DEPOSIT: return "Deposit";
        case TransactionType::WITHDRAW: return "Withdraw";
        case TransactionType::TRANSFER: return "Transfer";
        default: return "Unknown";
    }
}

/**
 * Log a transaction to the transaction log file
 * 
 * Format:
 * datetime,type,accountNumber,amount,username,status,secondAccount,details
 * 
 * @param type Transaction type (DEPOSIT, WITHDRAW, TRANSFER)
 * @param accountNumber Primary account involved
 * @param amount Transaction amount
 * @param username User who performed transaction
 * @param status Success/failure status
 * @param details Additional transaction details
 * @param secondAccountNumber Secondary account for transfers
 */
void logTransaction(TransactionType type,
                   int accountNumber,
                   double amount,
                   const string& username,
                   TransactionStatus status,
                   const string& details,
                   int secondAccountNumber) {
    
    ofstream logFile(TRANSACTION_LOG_FILE, ios::app);
    if (logFile.is_open()) {
        logFile << getCurrentDate() << ","
                << transactionTypeToString(type) << ","
                << accountNumber << ","
                << fixed << setprecision(2) << amount << ","
                << username << ","
                << (status == TransactionStatus::SUCCESS ? "Success" : "Failed") << ","
                << (secondAccountNumber != -1 ? to_string(secondAccountNumber) : "") << ","
                << details << endl;
        logFile.close();
    }
}

/**
 * Display transaction history for specific account(s)
 * 
 * Features:
 * - Formatted table display
 * - Account filtering
 * - Transaction details
 * - Success/failure status
 * 
 * @param accountNumber Account to show (-1 for all accounts)
 */
void viewTransactionHistory(int accountNumber) {
    ifstream logFile(TRANSACTION_LOG_FILE);
    string line;
    bool found = false;
    
    clearScreen();
    cout << "=== Transaction History ===" << endl;
    cout << setfill('=') << setw(100) << "=" << setfill(' ') << endl;
    
    // Display header
    cout << left 
         << setw(20) << "Date/Time" 
         << setw(10) << "Type"
         << setw(15) << "Account#"
         << setw(12) << "Amount"
         << setw(15) << "User"
         << setw(10) << "Status"
         << "Details" << endl;
    cout << setfill('-') << setw(100) << "-" << setfill(' ') << endl;

    // Process each transaction
    while (getline(logFile, line)) {
        istringstream iss(line);
        string datetime, type, accNum, amount, user, status, secAccNum, details;
        
        getline(iss, datetime, ',');
        getline(iss, type, ',');
        getline(iss, accNum, ',');
        getline(iss, amount, ',');
        getline(iss, user, ',');
        getline(iss, status, ',');
        getline(iss, secAccNum, ',');
        getline(iss, details);

        // Display if matches filter criteria
        if (accountNumber == -1 || accountNumber == stoi(accNum) || 
            (!secAccNum.empty() && accountNumber == stoi(secAccNum))) {
            cout << left
                 << setw(20) << datetime
                 << setw(10) << type
                 << setw(15) << accNum
                 << "$" << setw(11) << amount
                 << setw(15) << user
                 << setw(10) << status;
            
            if (type == "Transfer" && !secAccNum.empty()) {
                cout << "To Account#: " << secAccNum;
            }
            if (!details.empty()) {
                cout << " " << details;
            }
            cout << endl;
            found = true;
        }
    }

    // Display message if no transactions found
    if (!found) {
        cout << "No transactions found";
        if (accountNumber != -1) {
            cout << " for account #" << accountNumber;
        }
        cout << "." << endl;
    }

    cout << setfill('=') << setw(100) << "=" << setfill(' ') << endl;
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
