#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include <string>
#include <vector>
#include <memory>
#include "bankAccountType.h"
#include "userManagement.h"

void createAccount();
void displayMatchingUsers(const std::vector<User>& users, const std::vector<std::unique_ptr<bankAccountType>>& accounts);
void displayUserAccounts(const User& user, const std::vector<std::unique_ptr<bankAccountType>>& accounts);

#endif // CREATE_ACCOUNT_H
