/**
 * Account Lookup Header
 * 
 * Purpose:
 * Defines functionality for searching and retrieving accounts in the system.
 * Used primarily by managers to find accounts by name/account number.
 * 
 * Features:
 * - Partial name search
 * - Multiple match handling
 * - Account selection from search results
 */

#ifndef LOOK_UP_ACCOUNT_H
#define LOOK_UP_ACCOUNT_H

#include "bankAccountType.h"
#include <memory>

using namespace std;

// Search and return a single account, with user interaction for selection
unique_ptr<bankAccountType> lookUpAccount();

#endif // LOOK_UP_ACCOUNT_H
