#include "serviceChargeCheckingType.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

const double serviceChargeCheckingType::ACCOUNT_SERVICE_CHARGE = 10.00;
const int serviceChargeCheckingType::MAXIMUM_NUM_OF_CHECKS = 5;
const double serviceChargeCheckingType::SERVICE_CHARGE_EXCESS_NUM_OF_CHECKS = 5;

serviceChargeCheckingType::serviceChargeCheckingType(string n, int acctNumber, double bal)
    : checkingAccountType(n, acctNumber, bal)
{
    serviceChargeAccount = ACCOUNT_SERVICE_CHARGE;
    numberOfChecksWritten = 0;
    serviceChargeCheck = 0;
}

serviceChargeCheckingType::serviceChargeCheckingType(string n, int acctNumber, double bal, double monthlyFee)
    : checkingAccountType(n, acctNumber, bal)
{
    serviceChargeAccount = monthlyFee;
    numberOfChecksWritten = 0;
    serviceChargeCheck = 0;
}

double serviceChargeCheckingType::getServiceChargeAccount() const {
    return serviceChargeAccount;
}

void serviceChargeCheckingType::setServiceChargeAccount(double amount) {
    serviceChargeAccount = amount;
}

double serviceChargeCheckingType::getServiceChargeChecks() const {
    return serviceChargeCheck;
}

void serviceChargeCheckingType::setServiceChargeChecks(double amount) {
    serviceChargeCheck = amount;
}

int serviceChargeCheckingType::getNumberOfChecksWritten() const {
    return numberOfChecksWritten;
}

void serviceChargeCheckingType::setNumberOfChecksWritten(int num) {
    numberOfChecksWritten = num;
}

void serviceChargeCheckingType::postServiceCharge() {
    balance = balance - serviceChargeAccount;
}

void serviceChargeCheckingType::writeCheck(double amount) {
    if (numberOfChecksWritten < MAXIMUM_NUM_OF_CHECKS)
        balance = balance - amount;
    else
        balance = balance - amount - serviceChargeCheck;

    numberOfChecksWritten++;
}

void serviceChargeCheckingType::createMonthlyStatement() {
    postServiceCharge();
}

void serviceChargeCheckingType::print() {
    cout << fixed << showpoint << setprecision(2);
    cout << "Service Charge Checking: " << getName() << "\t ACCT# " << getAccountNumber()
         << "\tBalance: $" << getBalance();
}
