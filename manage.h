#pragma once

#include <iostream>
#include <string>
#include "accounts.h"
#include "accountsCredit.h"
#include <fstream>
#define FILENAME "database.txt"


class Manage {
public:
    Manage();
    ~Manage();

    void Add_Acc(const std::string& input);
    void Show_Acc();
    void Del_Acc(const std::string& input);
    void Mod_Acc(const std::string& oldInput, const std::string& newInput);
    void Search_Acc(const std::string& input);


    int m_Num;
    Accounts** m_Array;

    int IsExist(const std::string& name);
    void save();
    int get_Num();
    void init_Emp();
};

