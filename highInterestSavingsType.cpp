/**
 * High Interest Savings Account Implementation
 * 
 * This file implements the enhanced savings account features including:
 * - Higher interest earnings
 * - Minimum balance enforcement
 * - Withdrawal restrictions
 */

#include "highInterestSavingsType.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Initialize static class constants
const double highInterestSavingsType::MINIMUM_BALANCE = 2500.00;  // $2,500 minimum
const double highInterestSavingsType::INTEREST_RATE = 0.005;      // 0.5% interest

/**
 * Constructor for high interest savings account
 * 
 * Process:
 * 1. Calls savings account constructor with higher interest rate
 * 2. Sets minimum balance requirement
 * 
 * @param n Account holder's name
 * @param acctNumber Account identifier
 * @param bal Initial balance
 */
highInterestSavingsType::highInterestSavingsType(string n, int acctNumber, double bal)
    : savingsAccountType(n, acctNumber, bal, INTEREST_RATE) {
    minimumBalance = MINIMUM_BALANCE;
}

/**
 * Get minimum balance requirement
 * 
 * Used for:
 * - Account verification
 * - Customer information
 * - Withdrawal calculations
 * 
 * @return double The minimum balance requirement
 */
double highInterestSavingsType::getMinimumBalance() const {
    return minimumBalance;
}

/**
 * Verify if withdrawal would maintain minimum balance
 * 
 * Process:
 * 1. Calculates resulting balance after withdrawal
 * 2. Compares with minimum balance requirement
 * 
 * @param amount The amount to be withdrawn
 * @return bool True if withdrawal would maintain minimum balance
 */
bool highInterestSavingsType::verifyMinimumBalance(double amount) {
    return (getBalance() - amount >= minimumBalance);
}

/**
 * Process a withdrawal with minimum balance check
 * 
 * Steps:
 * 1. Verify minimum balance would be maintained
 * 2. If verified, process withdrawal
 * 3. If not verified, display error message
 * 
 * @param amount Amount to withdraw
 */
void highInterestSavingsType::withdraw(double amount) {
    if (verifyMinimumBalance(amount)) {
        bankAccountType::withdraw(amount);
    } else {
        cout << "Withdrawal not allowed. Balance would fall below minimum balance." << endl;
    }
}

/**
 * Display account information
 * 
 * Shows:
 * - Account type (High Interest Savings)
 * - Account holder
 * - Account number
 * - Current balance
 * 
 * Format:
 * Uses fixed point notation with 2 decimal places
 * Aligns output for readability
 */
void highInterestSavingsType::print() {
    cout << fixed << showpoint << setprecision(2);
    cout << "High Interest Savings: " << getName() 
         << "\t ACCT# " << getAccountNumber() 
         << "\t Balance: $" << getBalance();
}
