

#include <gtest/gtest.h>
#include "portfolio.hpp"   // علشان نستخدم Portfolio و AccountSettings

// 🧪 اختبار بسيط لإضافة حساب جديد
TEST(PortfolioTest, AddAccountSuccessfully) {
    Portfolio pf;

    AccountSettings settings;
    settings.type = AccountType::Checking;  // نوع الحساب
    settings.apr = 0.05;                    // فائدة
    settings.fee_flat_cents = 100;          // رسوم

    // جرب نضيف حساب جديد
    bool result = pf.add_account("CHK-001", settings, 10000);

    EXPECT_TRUE(result);   // المفروض ترجع true
}

// 🧪 اختبار حالة تكرار نفس الحساب
TEST(PortfolioTest, AddDuplicateAccountFails) {
    Portfolio pf;

    AccountSettings settings;
    settings.type = AccountType::Checking;
    settings.apr = 0.05;
    settings.fee_flat_cents = 100;

    // أول مرة تضاف بنجاح
    pf.add_account("CHK-001", settings, 10000);
    // تاني مرة المفروض تفشل
    bool result = pf.add_account("CHK-001", settings, 20000);

    EXPECT_FALSE(result);   // المفروض ترجع false
}

