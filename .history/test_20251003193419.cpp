#include <gtest/gtest.h>

#include "portfolio.hpp"  
#include "cal.hpp" 

// Test adding a new account successfully
TEST(PortfolioTest, AddAccountSuccessfully) {
    Portfolio pf;

    AccountSettings settings;
    settings.type = AccountType::Checking;  
    settings.apr = 0.05;                    
    settings.fee_flat_cents = 100;          

    bool result = pf.add_account("CHK-001", settings, 10000);

    EXPECT_TRUE(result);   
}

// Test adding a duplicate account (should fail)
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

// Test getting an existing account 
TEST(PortfolioTest, GetAccountSuccessfully)
{
    Portfolio pf;

    // Add the account first
    AccountSettings settings;
    settings.type = AccountType::Checking;
    settings.apr = 0.05;
    settings.fee_flat_cents = 100;
    pf.add_account("CHK-001", settings, 10000);

    // Now try to get it
    IAccount* acc = pf.get_account("CHK-001");


    EXPECT_NE(acc, nullptr);
    EXPECT_EQ(acc->id(), "CHK-001");
}

// Test getting a non-existent account
TEST(PortfolioTest, GetAccount_ReturnsNull_WhenAccountDoesNotExist) {
    Portfolio pf;

    IAccount* acc = pf.get_account("LOL-00");

    EXPECT_EQ(acc, nullptr);
}


class PortfolioTestFixture : public ::testing::Test {
protected:
    Portfolio pf;
    void SetUp() override {
        AccountSettings settings;
        settings.type = AccountType::Checking;
        settings.apr = 0.05;
        settings.fee_flat_cents = 100;
        pf.add_account("CHK-001", settings, 10000);
    }
};

TEST_F(PortfolioTestFixture, GetAccountSuccessfully) {
    IAccount* acc = pf.get_account("CHK-001");
    EXPECT_NE(acc, nullptr);
    EXPECT_EQ(acc->id(), "CHK-001");
}


TEST_F(PortfolioTestFixture, ApplyAllSuccessfully)
{
    TxRecord tx;
    tx.kind = TxKind::Deposit;
    tx.account_id = "CHK-001";
    tx.amount_cents = 700;
    tx.timestamp = 202509121023;
    tx.note = "Test deposit ";

    pf.apply_all({tx});

    IAccount* acc = pf.get_account("CHK-001");

    EXPECT_EQ(acc->balance_cents(), 10700);

}

TEST_F(PortfolioTestFixture, ApplyAllMissingAccount)
{
    TxRecord tx;
    tx.kind = TxKind::Deposit;
    tx.account_id = "LOL-001";
    tx.amount_cents = 700;
    tx.timestamp = 202509121023;
    tx.note = "Missing account ";

    pf.apply_all({tx});

    EXPECT_EQ(pf.get_account("LOL-001"), nullptr);
}

void Portfolio::apply_from_ledger(const string *tx_account_id,
                                  const int *tx_type,
                                  const long long *tx_amount_cents,
                                  int tx_count) {
  vector<TxRecord> vect;
  for (auto i = 0; i < tx_count; i++) {
    vect.push_back(
        {(TxKind)tx_type[i], tx_amount_cents[i], 0, "", tx_account_id[i]});
  }
  apply_all(vect);
}

TEST_F(PortfolioTestFixture, apply_from_ledger)
{
    TxRecord tx;
    tx.kind = TxKind::Deposit;
    tx.account_id = "LOL-001";
    tx.amount_cents = 700;
    tx.timestamp = 202509121023;
    tx.note = "Missing account ";

    pf.apply_all({tx});

    EXPECT_EQ(pf.get_account("LOL-001"), nullptr);
}