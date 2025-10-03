#ifndef _CAL_CPP
#define _CAL_CPP

#include <string>
using std::string;

/****************** 1.Balance Operations ******************/
/**
 * @brief Apply Deposits to the account
 * 
 * @param balance The current account balance in cents
 * @param amount  The amount to deposit in cents
 * @return The new balance after applying the deposit
 */
int        apply_deposit(int balance, unsigned int amount);

/**
 * @brief Apply withdrawal to the account
 * 
 * @param balance The current account balance in cents
 * @param amount  The amount to withdrawal in cents
 * @return The new balance after applying the withdrawal (may be negative)
 */
signed int apply_withdrawal(int balance, unsigned int amount);

/**
 * @brief Apply fee to the account
 * 
 * @param balance The current account balance in cents
 * @param fee     The fee amount in cents
 * @return The new balance after applying the fee
 */
signed int apply_fee(int balance, int fee);

/***********************  2.Interest  *********************/
/**
 * @brief Calculates simple interest for a given principal over a specified period
 * 
 * @param base              The amount in cents
 * @param apr               Annual Percentage Rate 
 * @param days              Number of days for interest
 * @param day_count_basis   The number of year days used for interest (365 or 360).
 * @param rounding_policy   Rounding policy forthe result 
 * @return The calculated simple interest amount

 */
int simple_interest(int base,float apr,int days,int day_count_basis,char rounding_policy);

/**
 * @brief Calculates compound amount over a specified period.
 * 
 * @param base              The amount in cents
 * @param apr               Annual Percentage Rate 
 * @param days              Number of days for compounding.
 * @param day_count_basis   Number of days considered in a year (365 or 360).
 * @param rounding_policy   Rounding policy for the result 
 * @return The total amount after applying compound interest
 */
int compound_amount(int base,float apr,int days,int day_count_basis,char rounding_policy);

/*****************  3. Currency Conversion  ***************/

/**
 * @brief Converts an amount from one currency to another.
 *
 * @param amount          The amount to convert 
 * @param from_currency   The source currency code 
 * @param to_currency     The target currency code 
 * @param rounding        Rounding policy to apply during conversion.
 * @return The converted amount in the target currency, rounded as specified.
 */
int fx_convert(int amount,string from_currency,string to_currency,char rounding);

/**********************  4. Utilities  ********************/
int  percent_of(int amount, double rate, char rounding_policy);
bool validate_rate(float rate);
bool validate_non_negative(signed int amount);

#endif /*_CAL_CPP*/


