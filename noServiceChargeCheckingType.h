/**
 * No Service Charge Checking Account Class Definition
 * 
 * Purpose:
 * Implements a checking account that:
 * - Has no monthly service fees
 * - Requires maintaining minimum balance
 * - Has interest earning capability
 * 
 * Inheritance Chain:
 * bankAccountType -> checkingAccountType -> noServiceChargeCheckingType
 * 
 * Features:
 * - Minimum balance requirement instead of fees
 * - Interest earning capability
 * - Check writing with balance verification
 * - Free check writing when balance maintained
 */

#ifndef noServiceChargeCheckingType_H
#define noServiceChargeCheckingType_H

#include <string>
#include "checkingAccountType.h"

using namespace std;

class noServiceChargeCheckingType: public checkingAccountType {
public:
    // Initialize with default minimum balance and interest rate
    noServiceChargeCheckingType(string n, int acctNumber, double bal);

    // Initialize with custom minimum balance and interest rate
    noServiceChargeCheckingType(string n, int acctNumber, double bal, 
                               double minBalance, double intRate);

    // Minimum balance operations
    double getMinimumBalance();
    void setMinimumBalance(double minBalance);
    bool verifyMinimumBalance(double minBalance);

    // Check writing and withdrawal operations
    void writeCheck(double amount) override;
    void withdraw(double amount);

    // Monthly statement and display
    virtual void createMonthlyStatement();
    virtual void print();

protected:
    double minimumBalance;  // Required minimum balance
    double interestRate;    // Current interest rate

private:
    static const double MIN_BALANCE;     // Default minimum balance ($1000.00)
    static const double INTEREST_RATE;   // Default interest rate (0.02 or 2%)
};

#endif
