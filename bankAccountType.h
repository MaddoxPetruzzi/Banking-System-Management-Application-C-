/**
 * Bank Account Base Class Definition
 * 
 * Purpose:
 * This is the abstract base class for all bank account types in the system.
 * It defines the common interface and basic functionality that all bank accounts must have.
 * 
 * Account Features:
 * - Basic account information (name, number, balance)
 * - Standard banking operations (deposit, withdraw)
 * - Monthly statement processing
 * - Account information display
 * 
 * Usage:
 * This class is inherited by specific account types such as:
 * - Checking accounts
 * - Savings accounts
 * - Certificate of deposit accounts
 * Each derived class implements its own specific features and restrictions
 */

#ifndef BANK_ACCOUNT_TYPE_H
#define BANK_ACCOUNT_TYPE_H

#include <string>

using namespace std;

class bankAccountType {
protected:
    string name;            // Account holder's name
    int accountNumber;      // Unique account identifier
    double balance;         // Current account balance

public:
    // Constructor to initialize account with basic information
    bankAccountType(string n = "", int acctNumber = 0, double bal = 0);

    // Basic account information retrieval
    int getAccountNumber() const;
    double getBalance() const;
    string getName() const;

    // Account information modification
    void setName(string n);
    void setBalance(double bal);

    // Basic banking operations
    virtual void withdraw(double amount);  // Virtual to allow overriding by derived classes
    void deposit(double amount);

    // Pure virtual functions that must be implemented by derived classes
    virtual void createMonthlyStatement() = 0;  // Monthly account maintenance
    virtual void print();                       // Account information display
    virtual string getType() const = 0;         // Returns account type identifier
};

#endif // BANK_ACCOUNT_TYPE_H
