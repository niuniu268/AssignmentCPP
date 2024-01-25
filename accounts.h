#pragma once

#include<iostream>
#include<string>

class Accounts {
public:
    virtual void showInfo() = 0;
    std::string m_Name;
    virtual ~Accounts() {}
};