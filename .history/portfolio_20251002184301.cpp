#include "cal.hpp"
#include "portfolio.hpp"
#include <iostream>
#include <cmath>

/**
 * @class 
 */
        
/**********  BaseAccount (public IAccount) **********/
BaseAccount::BaseAccount(const string& id, const AccountSettings& settings, long long opening_balance)
: id_(id), settings_(settings), balance_cents_(opening_balance){}

const string& BaseAccount::id() const
{
    return id_;
}

long long  BaseAccount::balance_cents() const
{
    return balance_cents_;
}

const vector<TxRecord>& BaseAccount::audit() const
{
    return audit_;
}
void BaseAccount::record(const TxRecord& tx)
{
    if(audit_.size() >= kMaxAudit)
    {
        audit_.erase(audit_.begin());
    }
    else{}
    audit_.push_back(tx);

}

void BaseAccount::deposit(long long amount_cents, long long ts, string note) 
{
    balance_cents_ = apply_deposit(balance_cents_, amount_cents);
    record({TxKind::Deposit, amount_cents, ts, note, id_});
}

void BaseAccount::withdrawal(long long amount_cents, long long ts, string note)
{
    balance_cents_ = apply_withdrawal(balance_cents_, amount_cents);
    record({TxKind::Withdrawal, amount_cents, ts, note, id_});
}

void BaseAccount::charge_fee(long long fee_cents, long long ts, string note )
{
    balance_cents_ = apply_fee(balance_cents_, fee_cents);
    record({TxKind::Fee, fee_cents, ts, note, id_});
}

void BaseAccount::post_simple_interest(int days, int basis, long long ts,string note)
{
    long long interest = simple_interest(balance_cents_, settings_.apr,days,basis,'E'); // Defult(Half Even)
    balance_cents_ += interest;
    record({TxKind::Interest, interest, ts, note, id_});
}

void BaseAccount::apply(const TxRecord& tx)
{
    switch(tx.kind)
    {
        case TxKind::Deposit:
            deposit(tx.amount_cents, tx.timestamp, tx.note); 
        break;
        case TxKind::Withdrawal:
            withdrawal(tx.amount_cents, tx.timestamp, tx.note);
        break;
        case TxKind::Fee:
            charge_fee(tx.amount_cents, tx.timestamp, tx.note);
        break;
        case TxKind::Interest:
            post_simple_interest(30, 365, tx.timestamp, tx.note);
        break;
        case TxKind::TransferIn: 
            deposit(tx.amount_cents, tx.timestamp, tx.note);
        break;
        case TxKind::TransferOut: 
            withdrawal(tx.amount_cents, tx.timestamp, tx.note);
        break;

    }
}

/**********  CheckingAccount **********/
CheckingAccount::CheckingAccount (const string& id, const AccountSettings& settings, long long opening_balance)
    :BaseAccount(id, settings, opening_balance){}

AccountType CheckingAccount::type() const
{
    return AccountType::Checking;
}

void CheckingAccount::apply_monthly_fee(long long ts)
{
    charge_fee(settings_.fee_flat_cents, ts, " monthly fee.");
}

/**********  SavingsAccount **********/
SavingsAccount::SavingsAccount (const string& id, const AccountSettings& settings, long long opening_balance)
    :BaseAccount(id, settings, opening_balance){}

AccountType SavingsAccount::type() const 
{
    return AccountType::Savings;
}

void SavingsAccount::post_interest(int days, int basis, long long ts)
{
    post_simple_interest(days, basis, ts,"posting interest monthly.");
}

/**********  Portfolio **********/
bool Portfolio::add_account(const string& id, const AccountSettings& settings, long long opening_balance)
{
    if(!accounts_.count(id))
    {
        if(settings.type == AccountType::Checking)
        {
            accounts_[id]= make_unique<CheckingAccount>(id, settings, opening_balance);
        }
        else
        {
            accounts_[id]= make_unique<SavingsAccount>(id, settings, opening_balance);
        }
        return true;
    }
    else
    {
        return false;
    }
}

IAccount* Portfolio::get_account(const string& id)
{
    auto iter = accounts_.find(id);
    if(iter != accounts_.end())
    {
        return iter->second.get();
    }
    else
    {
        return nullptr;
    }
}

void Portfolio::apply_all(const std::vector<TxRecord>& txs)
{
    for(auto& tx : txs) 
    {
        IAccount *account = get_account(tx.account_id);
        if(account)
        {
            account->apply(tx);
        }
        else
        {
            cout << "Missing account " << tx.account_id << endl;
        }
    }

}

void Portfolio::apply_from_ledger(const string* tx_account_id,const int* tx_type,const long long* tx_amount_cents, int tx_count) 
{
    vector<TxRecord> vect;
    for(auto i = 0; i< tx_count; i++)
    {
        vect.push_back({(TxKind)tx_type[i], tx_amount_cents[i], 0, "", tx_account_id[i]});
    }
    apply_all(vect);
}

bool Portfolio::transfer(const TransferRecord& tr)
{
    IAccount *from = get_account(tr.from_id);
    IAccount *to = get_account(tr.to_id);
    if(!(from||to))
    {
        return false;
    }
    else
    {
        from->apply({TxKind::TransferOut, tr.amount_cents, tr.timestamp, tr.note, tr.from_id});
        to->apply({TxKind::TransferIn, tr.amount_cents, tr.timestamp, tr.note, tr.to_id});
        return true;
    }
}

long long Portfolio::total_exposure() const
{
    long long total = 0;
    for(auto& i :accounts_)
    {
        total += i.second->balance_cents();
    }
    return total;
}







