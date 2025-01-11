/**
 * Certificate of Deposit Account Implementation
 * 
 * This file implements CD account features including:
 * - Time-based maturity tracking
 * - Interest calculations
 * - Withdrawal restrictions
 * - Monthly processing
 */

#include "certificateOfDepositType.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Initialize static constants
const double certificateOfDepositType::INTEREST_RATE = 0.05;              // 5% interest
const int certificateOfDepositType::NUMBER_OF_MATURITY_MONTHS = 6;        // 6-month term

/**
 * Default constructor with standard CD terms
 * 
 * Process:
 * 1. Initializes base account
 * 2. Sets default interest rate
 * 3. Sets default maturity period
 * 4. Initializes CD month counter
 * 
 * @param n Account holder's name
 * @param acctNumber Account identifier
 * @param bal Initial deposit
 */
certificateOfDepositType::certificateOfDepositType(string n, int acctNumber, double bal)
    : bankAccountType(n, acctNumber, bal)
{
    interestRate = INTEREST_RATE;
    maturityMonths = NUMBER_OF_MATURITY_MONTHS;
    cdMonth = 0;  // Start at month 0
}

/**
 * Constructor with custom CD terms
 * 
 * Used for:
 * - Special rate CDs
 * - Custom term lengths
 * - Promotional CDs
 * 
 * @param n Account holder's name
 * @param acctNumber Account identifier
 * @param bal Initial deposit
 * @param intRate Custom interest rate
 * @param maturityMon Custom maturity period
 */
certificateOfDepositType::certificateOfDepositType(string n, int acctNumber, double bal, double intRate, int maturityMon)
    : bankAccountType(n, acctNumber, bal)
{
    interestRate = intRate;
    maturityMonths = maturityMon;
    cdMonth = 0;
}

/**
 * Get current interest rate
 * @return double Current interest rate as decimal
 */
double certificateOfDepositType::getInterestRate() const {
    return interestRate;
}

/**
 * Update interest rate
 * @param rate New interest rate as decimal
 */
void certificateOfDepositType::setInterestRate(double rate) {
    interestRate = rate;
}

/**
 * Get current month in CD term
 * @return double Number of months since CD start
 */
double certificateOfDepositType::getCurrentCDMonth() const {
    return cdMonth;
}

/**
 * Update current month in CD term
 * @param month New current month
 */
void certificateOfDepositType::setCurrentCDMonth(int month) {
    cdMonth = month;
}

/**
 * Get total maturity period
 * @return int Total months until maturity
 */
int certificateOfDepositType::getMaturityMonths() const {
    return maturityMonths;
}

/**
 * Update maturity period
 * @param month New maturity period in months
 */
void certificateOfDepositType::setMaturityMonths(int month) {
    maturityMonths = month;
}

/**
 * Calculate and add monthly interest
 * 
 * Process:
 * Adds monthly interest to balance using CD rate
 */
void certificateOfDepositType::postInterest() {
    balance = balance + balance * interestRate;
}

/**
 * Standard withdrawal override
 * 
 * Note: No implementation as CDs use special withdrawal rules
 * @param amount Not used in CD accounts
 */
void certificateOfDepositType::withdraw(double amount) {
    // No implementation - CDs use special withdrawal rules
}

/**
 * CD-specific withdrawal processing
 * 
 * Rules:
 * - Only allow withdrawal after maturity
 * - Withdraw entire balance at maturity
 * - Display warning for early withdrawal attempt
 */
void certificateOfDepositType::withdraw() {
    if (cdMonth > maturityMonths)
        balance = 0;  // Full withdrawal at maturity
    else
        cout << "CD has not been matured. No withdrawal." << endl;
}

/**
 * Process monthly maintenance
 * 
 * Actions:
 * 1. Post monthly interest
 * 2. Increment CD month counter
 */
void certificateOfDepositType::createMonthlyStatement() {
    postInterest();
    cdMonth++;
}

/**
 * Display account information
 * 
 * Shows:
 * - Account type (Certificate of Deposit)
 * - Account holder
 * - Account number
 * - Current balance
 * 
 * Format:
 * Uses fixed point notation with 2 decimal places
 */
void certificateOfDepositType::print() {
    cout << fixed << showpoint << setprecision(2);
    cout << "Certificate of Deposit: " << getName() 
         << "\t ACCT# " << getAccountNumber() 
         << "\tBalance: $" << getBalance();
}
