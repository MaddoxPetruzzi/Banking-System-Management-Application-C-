/**
 * Bank Account Base Class Implementation
 * 
 * This file implements the basic functionality of a bank account.
 * All derived account types inherit and potentially override these functions.
 */

#include "bankAccountType.h"
#include <iostream>
#include <iomanip>

using namespace std;

/**
 * Constructor for creating a new bank account
 * 
 * Parameters:
 * @param n Account holder's name
 * @param acctNumber Unique account identifier
 * @param bal Initial account balance
 * 
 * Note: Default values are provided in the header for optional parameters
 */
bankAccountType::bankAccountType(string n, int acctNumber, double bal) {
    name = n;
    accountNumber = acctNumber;
    balance = bal;
}

/**
 * Get the account's unique identifier
 * 
 * @return int The account number
 */
int bankAccountType::getAccountNumber() const {
    return accountNumber;
}

/**
 * Get the current account balance
 * 
 * @return double The current balance
 */
double bankAccountType::getBalance() const {
    return balance;
}

/**
 * Get the account holder's name
 * 
 * @return string The account holder's name
 */
string bankAccountType::getName() const {
    return name;
}

/**
 * Update the account holder's name
 * 
 * @param n New name for the account holder
 */
void bankAccountType::setName(string n) {
    name = n;
}

/**
 * Update the account balance
 * Used for direct balance modifications (not typical transactions)
 * 
 * @param bal New balance for the account
 */
void bankAccountType::setBalance(double bal) {
    balance = bal;
}

/**
 * Basic withdrawal operation
 * 
 * Note: This is a virtual function that can be overridden by derived classes
 * to implement specific withdrawal rules (e.g., minimum balance requirements)
 * 
 * @param amount The amount to withdraw
 */
void bankAccountType::withdraw(double amount) {
    balance = balance - amount;
}

/**
 * Basic deposit operation
 * 
 * This function is not virtual as the deposit process is
 * consistent across all account types
 * 
 * @param amount The amount to deposit
 */
void bankAccountType::deposit(double amount) {
    balance = balance + amount;
}

/**
 * Print account information
 * 
 * Displays:
 * - Account holder's name
 * - Account number
 * - Current balance
 * 
 * Format:
 * Uses fixed point notation with 2 decimal places for currency
 */
void bankAccountType::print() {
    cout << fixed << showpoint << setprecision(2);
    cout << name << " " << accountNumber << " Balance : $" << balance;
}
