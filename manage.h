#pragma once

#include <iostream>
#include <string>
#include "accounts.h"
#include "accountsCredit.h"
#include <fstream>
#include <vector>
#define FILENAME "database.txt"


class Manage {
public:
    Manage();
    ~Manage();

    std::string Add_Acc(const std::string& input);
    void Show_Acc();
    std::string Del_Acc(const std::string& input);
    std::string Mod_Acc(const std::string& oldInput, const std::string& newInput);
    std::string Search_Acc(const std::string& input);


    int m_Num;
    Accounts** m_Array;

    int IsExist(const std::string& name);
    void save();
    int get_Num();
    void init_Emp();
};

