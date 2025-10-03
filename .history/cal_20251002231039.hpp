#ifndef _CAL_CPP
#define _CAL_CPP

#include <string>
using std::string;

/****************** 1.Balance Operations ******************/
/**
 *  @brief Deposits into the account
 * 
 * @param balance The current account balance in cents
 * @param amount  The amount to deposit in cents
 * @return 
 */
int        apply_deposit(int balance, unsigned int amount);
signed int apply_withdrawal(int balance, unsigned int amount);
signed int apply_fee(int balance, int fee);

/***********************  2.Interest  *********************/
int simple_interest(int base,float apr,int days,int day_count_basis,char rounding_policy);
int compound_amount(int base,float apr,int days,int day_count_basis,char rounding_policy);

/*****************  3. Currency Conversion  ***************/
int fx_convert(int amount,string from_currency,string to_currency,char rounding);

/**********************  4. Utilities  ********************/
int  percent_of(int amount, double rate, char rounding_policy);
bool validate_rate(float rate);
bool validate_non_negative(signed int amount);

#endif /*_CAL_CPP*/


