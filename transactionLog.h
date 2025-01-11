#ifndef TRANSACTION_LOG_H
#define TRANSACTION_LOG_H

// transactionLog.h handles all transaction logging and history viewing functionality.
// It tracks deposits, withdrawals, and transfers between accounts.

#include <string>
#include <vector>

using namespace std;

// Defines the different types of transactions that can be logged
enum class TransactionType {
    DEPOSIT,
    WITHDRAW,
    TRANSFER
};

// Defines the possible outcomes of a transaction
enum class TransactionStatus {
    SUCCESS,
    FAILED
};

// Records a transaction in the transaction log file
void logTransaction(TransactionType type,
                   int accountNumber,
                   double amount,
                   const string& username,
                   TransactionStatus status,
                   const string& details = "",
                   int secondAccountNumber = -1);

// Views transaction history for a specific account or all accounts if accountNumber is -1
void viewTransactionHistory(int accountNumber = -1);

// Views transaction history for a set of account numbers
void viewTransactionHistory(const vector<int>& accountNumbers);

// Views transaction history for a specific user
void viewUserTransactionHistory(const string& username);

#endif // TRANSACTION_LOG_H
