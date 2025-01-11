/**
 * Certificate of Deposit (CD) Account Class Definition
 * 
 * Purpose:
 * Implements a time-based deposit account that:
 * - Offers higher interest rates
 * - Has maturity period restrictions
 * - Penalizes early withdrawals
 * 
 * Inheritance Chain:
 * bankAccountType -> certificateOfDepositType
 * 
 * Features:
 * - Fixed maturity period
 * - Higher interest rates
 * - Withdrawal restrictions
 * - Maturity tracking
 */

#ifndef CERTIFICATE_OF_DEPOSIT_TYPE_H
#define CERTIFICATE_OF_DEPOSIT_TYPE_H

#include "bankAccountType.h"
#include <string>

using namespace std;

class certificateOfDepositType : public bankAccountType {
public:
    // Initialize CD with default interest rate and maturity
    certificateOfDepositType(string n, int acctNumber, double bal);

    // Initialize CD with custom interest rate and maturity
    certificateOfDepositType(string n, int acctNumber, double bal, double intRate, int maturityMon);

    // Interest rate management
    double getInterestRate() const;
    void setInterestRate(double rate);

    // CD month tracking
    double getCurrentCDMonth() const;
    void setCurrentCDMonth(int month);

    // Maturity period management
    int getMaturityMonths() const;
    void setMaturityMonths(int month);

    // Interest calculations
    void postInterest();

    // Withdrawal operations
    void withdraw(double amount) override;  // Override base class withdrawal
    void withdraw();                        // CD-specific withdrawal

    // Monthly maintenance
    void createMonthlyStatement() override;

    // Account information display
    void print() override;
    string getType() const override { return "Certificate of Deposit"; }

private:
    static const double INTEREST_RATE;               // Default interest rate (0.05 or 5%)
    static const int NUMBER_OF_MATURITY_MONTHS;     // Default maturity period (6 months)

    double interestRate;    // Account's interest rate
    int maturityMonths;     // Months until maturity
    int cdMonth;           // Current month in CD term
};

#endif // CERTIFICATE_OF_DEPOSIT_TYPE_H
