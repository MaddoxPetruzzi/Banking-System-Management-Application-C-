/**
 * High Interest Savings Account Class Definition
 * 
 * Purpose:
 * Extends the basic savings account with:
 * - Higher interest rates
 * - Minimum balance requirements
 * - Additional withdrawal restrictions
 * 
 * Key Features:
 * - Higher interest rate than regular savings
 * - Minimum balance enforcement
 * - Balance verification before withdrawals
 * - Special withdrawal rules
 */

#ifndef HIGH_INTEREST_SAVINGS_TYPE_H
#define HIGH_INTEREST_SAVINGS_TYPE_H

#include "savingsAccountType.h"
#include <string>

using namespace std;

class highInterestSavingsType : public savingsAccountType {
public:
    // Initialize account with higher interest rate and minimum balance requirement
    highInterestSavingsType(string n, int acctNumber, double bal);

    // Minimum balance operations
    double getMinimumBalance() const;
    bool verifyMinimumBalance(double amount);

    // Override withdrawal to enforce minimum balance
    void withdraw(double amount) override;

    // Display account information including minimum balance requirements
    void print() override;

    // Account type identifier
    string getType() const override { return "High Interest Savings"; }

private:
    // Constants for account rules
    static const double MINIMUM_BALANCE;  // Minimum required balance
    static const double INTEREST_RATE;    // Higher interest rate

    double minimumBalance;  // Current minimum balance requirement
};

#endif // HIGH_INTEREST_SAVINGS_TYPE_H
