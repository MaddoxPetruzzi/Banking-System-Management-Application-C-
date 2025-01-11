#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H

#include <string>
#include <vector>
#include <memory>
#include "bankAccountType.h"
#include "accountDatabase.h"
#include "transactionLog.h"
#include "loginLog.h"

using namespace std;

void changeUsernameMenu();
void changePasswordMenu(bool isManager = false);
void displayManagerMenu();
void displayClientMenu(const vector<int>& accountNumbers);
void displayLoginMenu(string& username, string& password);
void displayAccountOperationsMenu(int accountNumber);
unique_ptr<bankAccountType> selectAccount(const vector<int>& accountNumbers, const string& prompt);

#endif // MENU_FUNCTIONS_H
