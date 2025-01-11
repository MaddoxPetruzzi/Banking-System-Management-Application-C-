/**
 * Withdraw Operation Header
 * 
 * Purpose:
 * Defines the withdrawal functionality for the banking system.
 * Handles withdrawals for both manager operations and client self-service.
 * 
 * Usage:
 * - Client withdrawals: Pass specific account number
 * - Manager withdrawals: Pass -1 to search for account
 */

#ifndef WITHDRAW_H
#define WITHDRAW_H

void withdraw(int accountNumber = -1);  // -1 indicates manager operation

#endif // WITHDRAW_H
