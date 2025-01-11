/**
 * No Service Charge Checking Account Implementation
 * 
 * This file implements checking account features including:
 * - Minimum balance verification
 * - Free check writing
 * - Interest calculations
 * - Balance maintenance
 */

#include <iostream>
#include <iomanip>
#include <string>
#include "noServiceChargeCheckingType.h"

using namespace std;

// Initialize static constants
const double noServiceChargeCheckingType::MIN_BALANCE = 1000.0;   // $1,000 minimum
const double noServiceChargeCheckingType::INTEREST_RATE = 0.02;   // 2% interest

/**
 * Default constructor
 * 
 * Process:
 * 1. Initializes base checking account
 * 2. Sets default minimum balance
 * 3. Sets default interest rate
 * 
 * @param n Account holder's name
 * @param acctNumber Account identifier
 * @param bal Initial balance
 */
noServiceChargeCheckingType::noServiceChargeCheckingType(string n, int acctNumber, double bal)
                           : checkingAccountType(n, acctNumber, bal)
{
    minimumBalance = MIN_BALANCE;
    interestRate = INTEREST_RATE;
}

/**
 * Constructor with custom balance and rate
 * 
 * Used for:
 * - Special promotions
 * - Custom account arrangements
 * - VIP accounts
 * 
 * @param n Account holder's name
 * @param acctNumber Account identifier
 * @param bal Initial balance
 * @param minBalance Custom minimum balance requirement
 * @param intRate Custom interest rate
 */
noServiceChargeCheckingType::noServiceChargeCheckingType(string n, int acctNumber, double bal,
                                                       double minBalance, double intRate)
                           : checkingAccountType(n, acctNumber, bal)
{
    minimumBalance = minBalance;
    interestRate = intRate;
}

/**
 * Get minimum balance requirement
 * @return double Current minimum balance requirement
 */
double noServiceChargeCheckingType::getMinimumBalance()
{
    return minimumBalance;
}

/**
 * Update minimum balance requirement
 * @param minBalance New minimum balance requirement
 */
void noServiceChargeCheckingType::setMinimumBalance(double minBalance)
{
    minimumBalance = minBalance;
}

/**
 * Verify if withdrawal would maintain minimum balance
 * 
 * Process:
 * Calculates if balance after withdrawal would meet minimum
 * 
 * @param amount Amount to be withdrawn
 * @return bool True if minimum balance would be maintained
 */
bool noServiceChargeCheckingType::verifyMinimumBalance(double amount)
{
    return (balance - amount >= minimumBalance);
}

/**
 * Process check with minimum balance verification
 * 
 * Steps:
 * 1. Verify minimum balance would be maintained
 * 2. If verified, process check
 * 
 * @param amount Check amount
 */
void noServiceChargeCheckingType::writeCheck(double amount)
{
    if (verifyMinimumBalance(amount))
        balance = balance - amount;
}

/**
 * Process withdrawal with minimum balance verification
 * 
 * Steps:
 * 1. Verify minimum balance would be maintained
 * 2. If verified, process withdrawal
 * 
 * @param amount Withdrawal amount
 */
void noServiceChargeCheckingType::withdraw(double amount)
{
    if (verifyMinimumBalance(amount))
        balance = balance - amount;
}

/**
 * Process monthly account maintenance
 * 
 * Actions:
 * - Verify minimum balance maintenance
 * - Process any pending transactions
 */
void noServiceChargeCheckingType::createMonthlyStatement()
{
    // No monthly fees to process
    // Minimum balance verified during transactions
}

/**
 * Display account information
 * 
 * Shows:
 * - Account type (No Service Charge Checking)
 * - Account holder
 * - Account number
 * - Current balance
 * 
 * Format:
 * Uses fixed point notation with 2 decimal places
 */
void noServiceChargeCheckingType::print()
{
    cout << fixed << showpoint << setprecision(2);
    cout << "No Service Charge Check . " << getName() 
         << "\t ACCT# " << getAccountNumber() 
         << "\tBalance $" << getBalance();
}
