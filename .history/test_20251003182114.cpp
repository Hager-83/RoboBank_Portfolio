

#include <gtest/gtest.h>
#include "portfolio.hpp"   

// 🧪 اختبار بسيط لإضافة حساب جديد
TEST(PortfolioTest, AddAccountSuccessfully) {
    Portfolio pf;

    AccountSettings settings;
    settings.type = AccountType::Checking;  
    settings.apr = 0.05;                    
    settings.fee_flat_cents = 100;          

    bool result = pf.add_account("CHK-001", settings, 10000);

    EXPECT_TRUE(result);   
}

// 🧪 اختبار حالة تكرار نفس الحساب
TEST(PortfolioTest, AddDuplicateAccountFails) {
    Portfolio pf;

    AccountSettings settings;
    settings.type = AccountType::Checking;
    settings.apr = 0.05;
    settings.fee_flat_cents = 50;

    pf.add_account("CHK-001", settings, 10000);

    bool result = pf.add_account("CHK-001", settings, 20000);

    EXPECT_FALSE(result);   
}
/*
IAccount *Portfolio::get_account(const string &id) {
  auto iter = accounts_.find(id);
  if (iter != accounts_.end()) {
    return iter->second.get();
  } else {
    return nullptr;
  }
}*/
TEST(PortfolioTest, GetAccountSuccessfully)
{
    Portfolio pf;
    pf.get_account()

}