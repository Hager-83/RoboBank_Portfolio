#include "portfolio.hpp"
#include <iostream>

using namespace std;

int main() {
    Portfolio pf;

    /********** Create portfolio and accounts **********/
    AccountSettings chk_acc{AccountType::Checking, 0.0, 150};
    AccountSettings sav_acc{AccountType::Savings, 0.05, 0};

    pf.add_account("CHK-001", chk_acc, 0);
    pf.add_account("SAV-010", sav_acc, 500000);

        /********** Create portfolio and accounts **********/

    vector<TxRecord> txs;
    
    txs.push_back({TxKind::Deposit, 100000, 20250101, "Deposit", "CHK-001"});
    txs.push_back({TxKind::Withdrawal, 25000, 20250102, "Withdrawal", "CHK-001"});
    txs.push_back({TxKind::Fee, 1500, 20250103, "Withdrawal", "CHK-001"});
    pf.apply_all(txs);

    cout <<endl << "CHK-001 balance: " << pf.get_account("CHK-001")->balance_cents() <<endl;

    pf.get_account("SAV-010")->post_simple_interest(31, 365, 20250131, "monthly interest");
    cout <<endl << "SAV-010 balance: " << pf.get_account("SAV-010")->balance_cents() << endl;











    // 3. Savings interest

    // 4. Transfer
    TransferRecord tr{"SAV-010", "CHK-001", 30000, 20250201, "transfer"};
    pf.transfer(tr);
    cout << "CHK-001 after transfer: " << pf.get_account("CHK-001")->balance_cents() << "\n";
    cout << "SAV-010 after transfer: " << pf.get_account("SAV-010")->balance_cents() << "\n";

    // 5. Ledger path
    const int count = 2;
    string ids[count] = {"CHK-001", "SAV-010"};
    int types[count] = {0, 3};
    long long amounts[count] = {5000, 1000};
    pf.apply_from_ledger(ids, types, amounts, count);

    // 6. Total
    cout <<endl << "Total exposure: " << pf.total_exposure() << endl;

    return 0;
}
