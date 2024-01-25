#include "manage.h"
#include "accountsCredit.h"
#include <fstream>
#include <iostream>
#include <string>

Manage::Manage()  {

    std::ifstream ifs;
    ifs.open(FILENAME, std::ios::in);

    int num = this->get_Num();

    this->m_Num = num;
    this->m_Array = new Accounts *[this->m_Num];
    this->init_Emp();

}

Manage::~Manage() {

}

void Manage::Add_Acc(const std::string& input) {
    int newSize = m_Num + 1;
    Accounts** newSpace = new Accounts*[newSize];

    if (m_Array != nullptr) {
        for (int i = 0; i < m_Num; ++i) {
            newSpace[i] = m_Array[i];
        }
        delete[] m_Array;
    }

    Accounts* account = new AccountsCredits(input);
    newSpace[m_Num] = account;

    this->m_Array = newSpace;
    this->m_Num = newSize;
    this->save();

    std::cout << "success\n";
}

void Manage::Show_Acc() {
    if (m_Num == 0) {
        std::cout << "No accounts to display.\n";
    } else {
        for (int i = 0; i < m_Num; ++i) {
            m_Array[i]->showInfo();
        }
    }
}

void Manage::Del_Acc(const std::string& input) {

    int index = IsExist(input);

    if (index != -1) {
        delete m_Array[index];

        for (int i = index; i < m_Num - 1; ++i) {
            m_Array[i] = m_Array[i + 1];
        }

        this->m_Num--;
        this->save();

        std::cout << "success\n";

    } else {
        std::cout << "Account not found.\n";
    }
}

void Manage::Mod_Acc(const std::string& oldInput, const std::string& newInput) {
    int index = IsExist(oldInput);

    if (index != -1) {
        delete m_Array[index];

        Accounts* account = new AccountsCredits(newInput);
        account->m_Name = newInput;

        m_Array[index] = account;

        this->save();

        std::cout << "success\n";
    } else {
        std::cout << "Account not found.\n";
    }
}

void Manage::Search_Acc(const std::string& input) {
    int check = IsExist(input);

    if (check != -1) {
        std::cout << input << " exists\n";
    } else {
        std::cout << input << " does not exist\n";
    }
}

void Manage::save() {
    std::ofstream ofs;
    ofs.open(FILENAME, std::ios::out);

    for (int i = 0; i < this->m_Num; i++) {
        ofs << this->m_Array[i]->m_Name << std::endl;
    }
    ofs.close();
}

int Manage::get_Num() {
    std::ifstream ifs;
    ifs.open(FILENAME, std::ios::in);

    std::string name;
    int num = 0;
    while (ifs >> name) {
        num++;
    }
    return num;
}

void Manage::init_Emp() {
    std::ifstream ifs;
    ifs.open(FILENAME, std::ios::in);

    std::string name;

    int index = 0;

    while (ifs >> name) {
        Accounts* account = NULL;
        account = new AccountsCredits(name);
        this->m_Array[index] = account;
        index++;
    }
    ifs.close();
}

int Manage::IsExist(const std::string& name) {
    int index= -1;

    for (int i = 0; i < this->m_Num; i++) {
        if (this->m_Array[i]->m_Name == name) {
            index = i;
            break;
        }
    }
    return index;
}
