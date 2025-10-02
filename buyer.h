#ifndef BUYER_H
#define BUYER_H

#include <cstddef>
#include <string>
#include "bank_customer.h"

using namespace std;

class Buyer {
private:
    int id;
    string name;
    string address;
    string phoneNumber;
    BankCustomer* account; // Association with BankCustomer

public:
    Buyer(int id, const string& name, const string& address, const string& phone, BankCustomer* account0)
        : id(id), name(name), address(address), phoneNumber(phone), account(account0) {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getPhoneNumber() const { return phoneNumber; }
    BankCustomer* getAccount() { return account; }

    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }
    void setAddress(const string& newAddress) { address = newAddress; }
    void setPhoneNumber(const string& newPhone) { phoneNumber = newPhone; }
};

#endif // BUYER_H