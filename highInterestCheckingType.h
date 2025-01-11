/**
 * High Interest Checking Account Class Definition
 * 
 * Purpose:
 * Extends no service charge checking account with:
 * - Higher interest rate earnings
 * - Higher minimum balance requirements
 * - Interest calculation and posting
 * 
 * Inheritance Chain:
 * bankAccountType -> checkingAccountType -> noServiceChargeCheckingType -> highInterestCheckingType
 * 
 * Key Features:
 * - Highest interest rate among checking accounts
 * - Stricter minimum balance requirements
 * - Monthly interest payments
 */

#ifndef HIGHINTERESTCHECKINGTYPE_H
#define HIGHINTERESTCHECKINGTYPE_H

#include <string>
#include "noServiceChargeCheckingType.h"

using namespace std;

class highInterestCheckingType: public noServiceChargeCheckingType {
public:
    // Initialize with higher minimum balance and interest rate
    highInterestCheckingType(string n, int acctNumber, double bal);
    
    // Alternative constructor with custom minimum balance and interest rate
    highInterestCheckingType(string n, int acctNumber, double bal, double minBal, double intRate);

    // Interest rate management
    double getInterestRate();
    void setInterestRate(double intRate);

    // Interest calculation and posting
    void postInterest();

    // Monthly processing
    void createMonthlyStatement();

    // Account information display
    virtual void print();

private:
    static const double INTEREST_RATE;  // Default high interest rate (0.05 or 5%)
    static const double MIN_BALANCE;    // Default minimum balance ($5000)
};

#endif
