/**
 * Account Database Management System
 * 
 * Purpose:
 * Manages the persistent storage and operations for all bank accounts in the system.
 * Handles account creation, deletion, updates, and data persistence.
 * Works in conjunction with the user management system to maintain account relationships.
 * 
 * File Structure:
 * - Accounts stored in accounts.txt
 * - Uses encryption for data security
 * - Maintains atomic operations through file locking
 */

#ifndef ACCOUNT_DATABASE_H
#define ACCOUNT_DATABASE_H

#include <vector>
#include <memory>
#include "bankAccountType.h"

using namespace std;

// Saves a new or updated account to the database
// Uses file locking to ensure atomic operations
void saveAccount(const unique_ptr<bankAccountType>& account);

// Retrieves all accounts from the database
// Handles different account types and their specific attributes
vector<unique_ptr<bankAccountType>> loadAccounts();

// Generates the next available account number
// Ensures unique account numbers across the system
int getNextAccountNumber();

// Removes an account from the database
// Returns true if account was found and removed
bool removeAccountFromDatabase(int accountNumber);

// Updates an existing account's information
// Returns true if account was found and updated
bool updateAccountInDatabase(const bankAccountType& updatedAccount);

#endif // ACCOUNT_DATABASE_H
