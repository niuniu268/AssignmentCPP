#include "accountsCredit.h"

AccountsCredits::AccountsCredits(const std::string& name) {
    m_Name = name;
}

void AccountsCredits::showInfo() {
    std::cout << "account name: " << m_Name << std::endl;
}