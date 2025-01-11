/**
 * Service Charge Checking Account Implementation
 * 
 * This file implements checking account features including:
 * - Service charge calculations
 * - Check writing with limits
 * - Monthly fee processing
 */

#include "serviceChargeCheckingType.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Initialize static constants for account rules
const double serviceChargeCheckingType::ACCOUNT_SERVICE_CHARGE = 10.00;      // Monthly fee
const int serviceChargeCheckingType::MAXIMUM_NUM_OF_CHECKS = 5;             // Check limit
const double serviceChargeCheckingType::SERVICE_CHARGE_EXCESS_NUM_OF_CHECKS = 5.00;  // Excess check fee

/**
 * Default constructor with standard fees
 * 
 * Process:
 * 1. Initializes base checking account
 * 2. Sets up default service charges
 * 3. Initializes check counter
 * 
 * @param n Account holder's name
 * @param acctNumber Account identifier
 * @param bal Initial balance
 */
serviceChargeCheckingType::serviceChargeCheckingType(string n, int acctNumber, double bal)
    : checkingAccountType(n, acctNumber, bal)
{
    serviceChargeAccount = ACCOUNT_SERVICE_CHARGE;
    numberOfChecksWritten = 0;
    serviceChargeCheck = 0;
}

/**
 * Constructor with custom monthly fee
 * 
 * Used for:
 * - Special account arrangements
 * - Promotional rates
 * - Custom account packages
 * 
 * @param n Account holder's name
 * @param acctNumber Account identifier
 * @param bal Initial balance
 * @param monthlyFee Custom monthly service charge
 */
serviceChargeCheckingType::serviceChargeCheckingType(string n, int acctNumber, double bal, double monthlyFee)
    : checkingAccountType(n, acctNumber, bal)
{
    serviceChargeAccount = monthlyFee;
    numberOfChecksWritten = 0;
    serviceChargeCheck = 0;
}

/**
 * Get current monthly service charge
 * @return double Current monthly service charge
 */
double serviceChargeCheckingType::getServiceChargeAccount() const {
    return serviceChargeAccount;
}

/**
 * Update monthly service charge
 * @param amount New service charge amount
 */
void serviceChargeCheckingType::setServiceChargeAccount(double amount) {
    serviceChargeAccount = amount;
}

/**
 * Get current excess check fee
 * @return double Current excess check fee
 */
double serviceChargeCheckingType::getServiceChargeChecks() const {
    return serviceChargeCheck;
}

/**
 * Update excess check fee
 * @param amount New excess check fee
 */
void serviceChargeCheckingType::setServiceChargeChecks(double amount) {
    serviceChargeCheck = amount;
}

/**
 * Get number of checks written this month
 * @return int Number of checks written
 */
int serviceChargeCheckingType::getNumberOfChecksWritten() const {
    return numberOfChecksWritten;
}

/**
 * Update check counter
 * @param num New number of checks written
 */
void serviceChargeCheckingType::setNumberOfChecksWritten(int num) {
    numberOfChecksWritten = num;
}

/**
 * Apply monthly service charge to account
 * 
 * Process:
 * Deducts the monthly service fee from the balance
 */
void serviceChargeCheckingType::postServiceCharge() {
    balance = balance - serviceChargeAccount;
}

/**
 * Process check writing with fees
 * 
 * Process:
 * 1. If under check limit, just deduct amount
 * 2. If over limit, add excess check fee
 * 3. Update check counter
 * 
 * @param amount Check amount to process
 */
void serviceChargeCheckingType::writeCheck(double amount) {
    if (numberOfChecksWritten < MAXIMUM_NUM_OF_CHECKS)
        balance = balance - amount;
    else
        balance = balance - amount - serviceChargeCheck;

    numberOfChecksWritten++;
}

/**
 * Process monthly account maintenance
 * 
 * Actions:
 * 1. Apply monthly service charge
 * 2. Reset check counter for new month
 */
void serviceChargeCheckingType::createMonthlyStatement() {
    postServiceCharge();
}

/**
 * Display account information
 * 
 * Shows:
 * - Account type (Service Charge Checking)
 * - Account holder
 * - Account number
 * - Current balance
 * 
 * Format:
 * Uses fixed point notation with 2 decimal places
 */
void serviceChargeCheckingType::print() {
    cout << fixed << showpoint << setprecision(2);
    cout << "Service Charge Checking: " << getName() 
         << "\t ACCT# " << getAccountNumber()
         << "\tBalance: $" << getBalance();
}
