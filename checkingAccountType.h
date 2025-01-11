/**
 * Checking Account Base Class Definition
 * 
 * Purpose:
 * Serves as an abstract base class for all types of checking accounts.
 * Inherits from bankAccountType and adds checking-specific functionality.
 * 
 * Inheritance Hierarchy:
 * bankAccountType -> checkingAccountType -> specific checking account types
 * 
 * Features Added:
 * - Abstract check writing capability
 * - Specialized account type identification
 * - Base for various checking account implementations
 */

#ifndef CHECKING_ACCOUNT_TYPE_H
#define CHECKING_ACCOUNT_TYPE_H

#include "bankAccountType.h"

using namespace std;

class checkingAccountType : public bankAccountType {
public:
    // Constructor initializes basic checking account information
    checkingAccountType(string n, int acctNumber, double bal);

    // Pure virtual function that must be implemented by derived classes
    // Each type of checking account will have its own check writing rules
    virtual void writeCheck(double amount) = 0;

    // Identifies this as a checking type account
    string getType() const override { return "Checking"; }
};

#endif // CHECKING_ACCOUNT_TYPE_H
