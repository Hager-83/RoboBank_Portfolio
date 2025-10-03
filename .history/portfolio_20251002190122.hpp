#ifndef ROBOBANK_PORTFOLIO_H_
#define ROBOBANK_PORTFOLIO_H_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/********** Enumerations **********/
enum class AccountType
{
    Checking ,
    Savings ,
};

enum class TxKind
{
    Deposit ,
    Withdrawal ,
    Fee , 
    Interest , 
    TransferIn , 
    TransferOut ,
};

/********** Structs **********/
struct AccountSettings
{
    AccountType type; 
    double apr;
    long long fee_flat_cents;

};

struct TxRecord
{
    TxKind kind;
    long long amount_cents;
    long long timestamp;
    string note;
    string account_id;  /// Portfolio can route to the right account

};

struct TransferRecord
{
    string from_id;
    string to_id; 
    long long amount_cents;
    long long timestamp;
    string note;
};

/********** Interface (abstract base) **********/
class IAccount
{
    public:
        virtual const string& id() const = 0;
        virtual AccountType type() const = 0;
        virtual long long balance_cents() const = 0;
        virtual void deposit(long long amount_cents, long long ts, string note = nullptr) = 0;
        virtual void withdrawal(long long amount_cents, long long ts, string note = nullptr) = 0;
        virtual void charge_fee(long long fee_cents, long long ts,  string note = nullptr) = 0;
        virtual void post_simple_interest(int days, int basis, long long ts, string note) = 0;
        virtual void apply(const TxRecord& tx) = 0;
        virtual const vector<TxRecord>& audit() const = 0;

        virtual ~IAccount() = default;

};

/**
 * @class BaseAccount
 * @brief A base class that shares common data and default behaviors for all account types.
 * 
 * 
 */
class  BaseAccount : public IAccount
{
    protected:
        string id_;
        AccountSettings settings_;
        long long balance_cents_;
        vector<TxRecord> audit_;
        static const long long kMaxAudit = 256; 

        /**
         * @brief Records a new transaction in the audit trail
         * 
         * The oldest transaction is removed when the number of stored transactions exceeds kMaxAudit.
         * 
         * @param tx The transaction record to be added
         */

        void record(const TxRecord& tx);

    public:
        /**
         * @brief Constructs a BaseAccount with ID, settings, 
         */
        BaseAccount(const string& id, const AccountSettings& settings, long long opening_balance = 0);

        virtual const string& id() const override;
        virtual long long balance_cents() const override;
        virtual const vector<TxRecord>& audit() const override;

        virtual void deposit(long long amount_cents, long long ts, string note = nullptr) override;
        virtual void withdrawal(long long amount_cents, long long ts, string note = nullptr) override;
        virtual void charge_fee(long long fee_cents, long long ts, string note = nullptr) override;
        virtual void post_simple_interest(int days, int basis, long long ts,string note) override;
        virtual void apply(const TxRecord& tx) override;
};

/**********  CheckingAccount **********/
class CheckingAccount : public BaseAccount
{
    public:
        CheckingAccount (const string& id, const AccountSettings& settings, long long opening_balance = 0);
        virtual AccountType type() const override;
        void apply_monthly_fee(long long ts);

};

/**********  SavingsAccount **********/
class SavingsAccount : public BaseAccount
{
    public:
        SavingsAccount (const string& id, const AccountSettings& settings, long long opening_balance = 0);
        virtual AccountType type() const override;
        void post_interest(int days, int basis, long long ts);

};

/**********  Portfolio **********/
class Portfolio 
{
    unordered_map<string , unique_ptr<IAccount>> accounts_;

    public:
        bool add_account (const string& id, const AccountSettings& settings, long long opening_balance = 0);
        IAccount* get_account(const string& id);
        void apply_all(const vector<TxRecord>& txs);
        void apply_from_ledger(const string* tx_account_id,const int* tx_type,const long long* tx_amount_cents, int tx_count);
        bool transfer(const TransferRecord& tr);
        long long total_exposure() const;
};

#endif /*ROBOBANK_PORTFOLIO_H_ */