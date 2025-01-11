/**
 * Deposit Operation Header
 * 
 * Purpose:
 * Defines the deposit functionality for the banking system.
 * Handles deposits for both manager operations and client self-service.
 * 
 * Usage:
 * - Client deposits: Pass specific account number
 * - Manager deposits: Pass -1 to search for account
 */

#ifndef DEPOSIT_H
#define DEPOSIT_H

void deposit(int accountNumber = -1);  // -1 indicates manager operation

#endif // DEPOSIT_H
