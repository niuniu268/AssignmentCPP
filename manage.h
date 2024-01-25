#pragma once

#include<iostream>
#include <string>
#include "accounts.h"
#include "accountsCredit.h"

class Manage {
public:
    Manage();
    void Add_Acc(const std::string& input);
    ~Manage();

private:
    int m_Num;
    Accounts** m_Array;
};