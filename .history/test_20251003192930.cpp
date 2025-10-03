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

TEST_F(PortfolioTestFixture, AnotherTest) {
    // pf here already has CHK-001 because of SetUp()
}
/*
void Portfolio::apply_all(const std::vector<TxRecord> &txs) {
  for (auto &tx : txs) {
    IAccount *account = get_account(tx.account_id);
    if (account) {
      account->apply(tx);
    } else {
      cout << "Missing account " << tx.account_id << endl;
*/
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
    tx.note = "Test deposit ";

    pf.apply_all({tx});

    IAccount* acc = pf.get_account("CHK-001");

    EXPECT_EQ(acc->balance_cents(), 10700);

}