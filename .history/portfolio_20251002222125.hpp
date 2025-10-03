#ifndef PORTFOLIO_HPP_
#define PORTFOLIO_HPP_

#include <cstdint>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/**
 * @enum AccountType
 * @brief Represents diffrent types of bank accounts
 */
enum class AccountType {
    Checking ,
    Savings ,
};

/**
 * @enum TxKind
 * @brief Represents diffrent types of transactions
 */
enum class TxKind {
    Deposit ,
    Withdrawal ,
    Fee ,
    Interest ,
    TransferIn ,
    TransferOut ,
};

/**
 * @struct AccountSettings
 * @brief Hold Account settings for specific account  
 */
struct AccountSettings {
    AccountType type;
    double apr;
    int64_t fee_flat_cents;
};

/**
 * @struct TxRecord
 * @brief Hold Account settings for specific account  
 */
struct TxRecord {
    TxKind kind;
    int64_t amount_cents;
    int64_t timestamp;
    string note;
    string account_id;  /// Portfolio can route to the right account
};

/**
 * @struct AccountSettings
 * @brief Hold Account settings for specific account  
 */
struct TransferRecord {
    string from_id;
    string to_id;
    int64_t amount_cents;
    int64_t timestamp;
    string note;
};

/**
 * @brief Interface for all account types
 */
class IAccount {
 public:
        
    /// @return The unique account identifier 
    virtual const string& id() const = 0;
        
    /// @return The account type
    virtual AccountType type() const = 0;
        
    /// @return The account balance in cents
    virtual long long balance_cents() const = 0;

    /**
     *  @brief Deposits into the account
     * 
     *  @param amount_cents Amount to deposit in cents
     *  @param ts Timestamp of the transaction
     *  @param note Transaction note (optional)
     */
    virtual void deposit(int64_t amount_cents, int64_t ts, string note = nullptr) = 0;

    /**
     *  @brief Withdrawal from the account
     * 
     *  @param amount_cents Amount to withdrawal in cents
     *  @param ts Timestamp of the transaction
     *  @param note Transaction note (optional)
     */
    virtual void withdrawal(int64_t amount_cents, int64_t ts, string note = nullptr) = 0;

    /**
     *  @brief Apply fee to the account
     * 
     *  @param fee_cents The fee amount in cents
     *  @param ts Timestamp of the transaction
     *  @param note Transaction note (optional)
     */
        virtual void charge_fee(int64_t fee_cents, int64_t ts,  string note = nullptr) = 0;

    /**
     *  @brief Post simple interest to account balance
     * 
     *  @param days Number of days for interest
     *  @param basis Interest basis 
     *  @param basis Timestamp of the transaction
     *  @param note Transaction note (optional)
     */
        virtual void post_simple_interest(int days, int basis, int64_t ts, string note) = 0;

    /**
     * @brief apply a transaction to the account
     * 
     * @param tx Transaction record to apply
     */
    virtual void apply(const TxRecord& tx) = 0;

    /**
     * @brief Returns the transaction audit history
     * 
     * @return Refrance to a vector of transaction audit history
     */
    virtual const vector<TxRecord>& audit() const = 0;

    virtual ~IAccount() = default;

};

/**
 * @class BaseAccount
 * @brief A base class that shares common data and default behaviors for all account types.
 */
class  BaseAccount : public IAccount {
 protected:
        string id_;
        AccountSettings settings_;
        int64_t balance_cents_;
        vector<TxRecord> audit_;
        static const int64_t kMaxAudit = 256; 

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
         * @brief Constructor of the BaseAccount 
         * 
         * @param id unique account identifier 
         * @param settings Account configuration settings
         * @param opening_balance Initial balance in cents (defaults to 0)
         */
        BaseAccount(const string& id, const AccountSettings& settings,
                    long long opening_balance = 0);

        /// @return  The unique account identifier
        const string& id() const override;

        /// @return  The account balance in cents
        long long balance_cents() const override;

        /// @return Refrane to transaction audit trail
        const vector<TxRecord>& audit() const override;

        /**
         *  @brief Deposits into the account
         * 
         *  @param amount_cents Amount to deposit in cents
         *  @param ts Timestamp of the transaction
         *  @param note Transaction note (optional)
         */
        void deposit(int64_t amount_cents, int64_t ts, string note = nullptr) override;

        /**
         *  @brief Withdrawal from the account
         * 
         *  @param amount_cents Amount to withdrawal in cents
         *  @param ts Timestamp of the transaction
         *  @param note Transaction note (optional)
         */
        void withdrawal(int64_t amount_cents, int64_t ts, string note = nullptr) override;

        /**
         *  @brief Apply fee to the account
         * 
         *  @param fee_cents The fee amount in cents
         *  @param ts Timestamp of the transaction
         *  @param note Transaction note (optional)
         */
        void charge_fee(int64_t fee_cents, int64_t ts,
                                string note = nullptr) override;

        /**
         *  @brief Post simple interest to account balance
         * 
         *  @param days Number of days for interest
         *  @param basis Interest basis 
         *  @param basis Timestamp of the transaction
         *  @param note Transaction note (optional)
         */
        void post_simple_interest(int days, int basis,
                                          int64_t ts, string note) override;

        /**
         * @brief apply a transaction to the account
         * 
         * @param tx Transaction record to apply
         */
        void apply(const TxRecord& tx) override;
};

/**********  CheckingAccount **********/
class CheckingAccount : public BaseAccount {
 public:
        CheckingAccount(const string& id, const AccountSettings& settings,
                         int64_t opening_balance = 0);
        AccountType type() const override;
        void apply_monthly_fee(long long ts);
};

/**********  SavingsAccount **********/
class SavingsAccount : public BaseAccount {
 public:
        SavingsAccount(const string& id, const AccountSettings& settings,
                       long long opening_balance = 0);
        AccountType type() const override;
        void post_interest(int days, int basis, int64_t ts);
};

/**********  Portfolio **********/
class Portfolio {
    unordered_map<string , unique_ptr<IAccount>> accounts_;

 public:

        bool add_account (const string& id, const AccountSettings& settings,
                          int64_t opening_balance = 0);
        IAccount* get_account(const string& id);
        void apply_all(const vector<TxRecord>& txs);
        void apply_from_ledger(const string* tx_account_id,const int* tx_type,
                               const int64_t* tx_amount_cents, int tx_count);
        bool transfer(const TransferRecord& tr);
        int64_t total_exposure() const;
};

#endif  // PORTFOLIO_HPP_
