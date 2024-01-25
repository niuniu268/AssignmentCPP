#pragma once

#include<iostream>
#include "accounts.h"

class AccountsCredits : public Accounts {
public:
    AccountsCredits(const std::string& name);
    virtual void showInfo();
};