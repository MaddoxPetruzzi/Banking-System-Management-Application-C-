/**
 * Savings Account Class Implementation
 * 
 * This file implements all savings account functionality including:
 * - Interest calculations
 * - Monthly processing
 * - Account information display
 */

#include "savingsAccountType.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

/**
 * Constructor for savings account
 * 
 * Initializes:
 * 1. Basic account info through base class
 * 2. Sets up interest rate for earnings
 * 
 * @param n Account holder's name
 * @param acctNumber Unique account identifier
 * @param bal Initial balance
 * @param intRate Interest rate as decimal (e.g., 0.05 for 5%)
 */
savingsAccountType::savingsAccountType(string n, int acctNumber, double bal, double intRate)
    : bankAccountType(n, acctNumber, bal), interestRate(intRate) {
}

/**
 * Get current interest rate
 * 
 * @return double Current interest rate as decimal
 */
double savingsAccountType::getInterestRate() const {
    return interestRate;
}

/**
 * Update account's interest rate
 * 
 * Used when:
 * - Bank changes rates
 * - Account type changes
 * - Special rate promotions
 * 
 * @param rate New interest rate as decimal
 */
void savingsAccountType::setInterestRate(double rate) {
    interestRate = rate;
}

/**
 * Calculate and add monthly interest to balance
 * 
 * Process:
 * 1. Calculate interest based on current balance
 * 2. Add interest amount to account balance
 * 
 * Formula: balance = balance + (balance * interestRate)
 */
void savingsAccountType::postInterest() {
    balance = balance + balance * interestRate;
}

/**
 * Process monthly account maintenance
 * 
 * Actions:
 * 1. Posts interest for the month
 * 2. Any additional monthly processing
 * 
 * Called:
 * - At end of each month
 * - When statement generation is requested
 */
void savingsAccountType::createMonthlyStatement() {
    postInterest();
}

/**
 * Display account information
 * 
 * Shows:
 * - Account type (Savings)
 * - Account holder name
 * - Account number
 * - Current balance
 * 
 * Format:
 * Uses fixed point notation with 2 decimal places
 * Aligns output for readability
 */
void savingsAccountType::print() {
    cout << fixed << showpoint << setprecision(2);
    cout << "Savings Account: " << getName() 
         << "\t ACCT# " << getAccountNumber() 
         << "\tBalance: $" << getBalance();
}
