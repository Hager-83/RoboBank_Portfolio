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

TEST_F(PortfolioTestFixture, ApplyFromLedgerSuccessfully)
{
    AccountSettings settings;
    settings.type = AccountType::Savings;
    settings.apr = 0.05;
    settings.fee_flat_cents = 100;
    pf.add_account("SAV-001", settings, 3000);

    // Prepare ledger arrays
    std::string ids[3] = {"CHK-001", "SAV-001", "LOL-001"};  
    int types[3] = {
        static_cast<int>(TxKind::Deposit),
        static_cast<int>(TxKind::Withdrawal),
        static_cast<int>(TxKind::Deposit)
    };
    long long amounts[3] = {500, 1000, 250};

    pf.apply_from_ledger(ids, types, amounts, 3);

    IAccount* CHKAcc = pf.get_account("CHK-001");
    ASSERT_NE(CHKAcc, nullptr);

    EXPECT_EQ(CHKAcc->balance_cents(), 10500);

    IAccount* SAVAcc = pf.get_account("SAV-001");
    ASSERT_NE(SAVAcc, nullptr);

    EXPECT_EQ(SAVAcc->balance_cents(), 2000);

    //Missing account
    IAccount* LOLAcc = pf.get_account("LOL-001");
    EXPECT_EQ(LOLAcc, nullptr);
}


TEST_F(PortfolioTestFixture,TransferSuccessfully)
{
    TransferRecord tr;
    tr.from = "CHK-001"; 
    tr.to =  "SAV-001";
    tr.amount_cents = 1200;
    tx.amount_cents = 700;
    tr.timestamp = 202510030000;
    tr.note = "Test transfer";

    bool result = pf.transfer(tr);
    EXPECT_TRUE(result);
}

TEST_F(PortfolioTestFixture,TransferSuccessfully)
{
    TransferRecord tr;
    tr.from_id = "LOL-001"; 
    tr.to_id =  "SAV-001";
    tr.amount_cents = 1200;
    tr.timestamp = 202510030000;
    tr.note = "Test transfer";

    bool result = pf.transfer(tr);
    EXPECT_FALSE(result);

    tr.to =  "LOL-002";
    result = pf.transfer(tr);
    EXPECT_FALSE(result);
}