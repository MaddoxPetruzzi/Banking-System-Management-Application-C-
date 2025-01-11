#ifndef SERVICE_CHARGE_CHECKING_TYPE_H
#define SERVICE_CHARGE_CHECKING_TYPE_H

#include "checkingAccountType.h"
#include <string>

using namespace std;

class serviceChargeCheckingType: public checkingAccountType {
public:
    serviceChargeCheckingType(string n, int acctNumber, double bal);
    serviceChargeCheckingType(string n, int acctNumber, double bal, double monthlyFee);

    double getServiceChargeAccount() const;
    void setServiceChargeAccount(double amount);
    double getServiceChargeChecks() const;
    void setServiceChargeChecks(double amount);
    int getNumberOfChecksWritten() const;
    void setNumberOfChecksWritten(int num);
    void postServiceCharge();
    void writeCheck(double amount) override;
    void createMonthlyStatement() override;
    void print() override;
    string getType() const override { return "Service Charge Checking"; }

protected:
    double serviceChargeAccount;
    double serviceChargeCheck;
    int numberOfChecksWritten;

private:
    static const double ACCOUNT_SERVICE_CHARGE;
    static const int MAXIMUM_NUM_OF_CHECKS;
    static const double SERVICE_CHARGE_EXCESS_NUM_OF_CHECKS;
};

#endif // SERVICE_CHARGE_CHECKING_TYPE_H
