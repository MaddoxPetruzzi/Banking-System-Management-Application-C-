/**
 * Account Listing Header
 * 
 * Purpose:
 * Defines functionality for displaying account lists.
 * Supports both manager (all accounts) and client (personal accounts) views.
 * 
 * Features:
 * - Full account listing
 * - Filtered account viewing
 * - Balance summaries
 * - Formatted display
 */

#ifndef LIST_ALL_ACCOUNTS_H
#define LIST_ALL_ACCOUNTS_H

#include <vector>

using namespace std;

// View accounts with optional filtering by account numbers
void listAllAccounts(const vector<int>& userAccounts = vector<int>());

#endif // LIST_ALL_ACCOUNTS_H
