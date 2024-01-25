#include "manage.h"

Manage::Manage() : m_Num(0), m_Array(nullptr) {}

Manage::~Manage() {
    // Cleanup code if needed...
}

void Manage::Add_Acc(const std::string& input) {
    int newSize = m_Num + 1;
    Accounts** newSpace = new Accounts*[newSize];

    if (m_Array != nullptr) {
        for (int i = 0; i < m_Num; i++) {
            newSpace[i] = m_Array[i];
        }
    }

    Accounts* account = new AccountsCredits(input);
    newSpace[m_Num] = account;

    delete[] m_Array;
    m_Array = newSpace;
    m_Num = newSize;

    std::cout << "success\n";
}