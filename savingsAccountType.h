/**
 * Savings Account Class Definition
 * 
 * Purpose:
 * Defines a basic savings account with interest earning capability.
 * Inherits from bankAccountType and adds savings-specific features.
 * 
 * Features:
 * - Interest rate management
 * - Monthly interest posting
 * - Savings-specific statement creation
 * - Balance maintenance with interest calculations
 */

#ifndef SAVINGS_ACCOUNT_TYPE_H
#define SAVINGS_ACCOUNT_TYPE_H

#include "bankAccountType.h"
#include <string>

using namespace std;

class savingsAccountType : public bankAccountType {
public:
    // Initialize savings account with interest rate
    savingsAccountType(string n, int acctNumber, double bal, double intRate);

    // Interest rate management
    double getInterestRate() const;
    void setInterestRate(double rate);

    // Interest calculation and posting
    void postInterest();

    // Monthly maintenance and statement creation
    void createMonthlyStatement() override;

    // Account information display
    void print() override;

    // Account type identification
    string getType() const override { return "Savings"; }

protected:
    double interestRate;  // Interest rate as a decimal (e.g., 0.05 for 5%)
};

#endif // SAVINGS_ACCOUNT_TYPE_H
