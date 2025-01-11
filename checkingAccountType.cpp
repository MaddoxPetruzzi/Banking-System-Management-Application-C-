/**
 * Checking Account Base Class Implementation
 * 
 * This file implements the base functionality for checking accounts.
 * Most implementation is handled by derived classes, this provides
 * the foundation and common elements.
 */

#include <iostream>
#include <string>
#include "checkingAccountType.h"

using namespace std;

/**
 * Constructor for checking accounts
 * 
 * Calls the base class (bankAccountType) constructor to initialize:
 * - Account holder's name
 * - Account number
 * - Initial balance
 * 
 * Parameters:
 * @param n Account holder's name
 * @param acctNumber Unique account identifier
 * @param bal Initial account balance
 * 
 * Note: This constructor only handles basic initialization.
 * Specific checking account types may add their own initialization.
 */
checkingAccountType::checkingAccountType(string n, int acctNumber, double bal)
                    : bankAccountType(n, acctNumber, bal)
{
    // No additional initialization needed at this level
    // Derived classes will add their specific initialization
}

// writeCheck is pure virtual (=0) and implemented by derived classes
// Each type of checking account has its own rules for check writing:
// - Service charge checking may have fees
// - No service charge checking may have minimum balance requirements
// - High interest checking may have both interest and balance requirements
