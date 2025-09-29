#include <iostream>
#include <cmath>
#include "cal.hpp"

using namespace std;


/****************** 1.Balance Operations ******************/
int apply_deposit(int balance, unsigned int amount)
{
    //cout << "Adding " << amount << " to balance."<<endl;   
    balance += amount;
    return balance;
}

signed int apply_withdrawal(int balance, unsigned int amount)
{
    //cout << "Withdrawing " << amount << " from balance."<<endl;
    balance -= amount;
    return balance;
}

signed int apply_fee(int balance, int fee)
{
    cout << "Applying fee of " << fee << endl;
    balance -= fee;
    return balance;
}

/*********************** 2.Interest ***********************/
int simple_interest(int base,float apr,int days,int day_count_basis,char rounding_policy)
{
    double interest = 0;
    interest = base*apr*((double)days/day_count_basis);
    if('U' == rounding_policy)
    {
        /* Half UP */
        interest = round(interest);
    }
    else if('E' == rounding_policy )
    {
        /* Half Even */
        interest = nearbyint(interest);
    }
    else{/* Nothing */}
    return (int)interest;
}

int compound_amount(int principal,float apr,int years,int compounds_per_year,char rounding_policy)
{
    double  total_amount  = 0;
    double rate = years * compounds_per_year;
    double base = (apr/compounds_per_year) + 1;
    total_amount  = principal * pow(base, rate);
    if('U' == rounding_policy)
    {
        /* Half UP */
        total_amount = round(total_amount);
    }
    else if('E' == rounding_policy )
    {
        /* Half Even */
        total_amount = nearbyint(total_amount);
    }
    else{/* Nothing */}
    return (int)total_amount;
}

/*****************  3. Currency Conversion  ***************/

int fx_convert(int amount,string from_currency,string to_currency,char rounding_policy)
{
    double fx_table[5] = {1.0, 0.91, 3.75, 48.0, 147.0};

    double converted_amount = 0;
    /* from EUR */
    if("EUR" == from_currency)
    {
        if("EUR" == to_currency)
        {
            converted_amount = amount;
        }
        else if("USD" == to_currency)
        {
            converted_amount = ((amount * fx_table[0])/fx_table[1]) ;
        }
        else if("SAR" == to_currency)
        {
            converted_amount = ((amount*fx_table[2])/fx_table[1]);
        }
        else if("EGP" == to_currency)
        {
            converted_amount = ((amount*fx_table[3])/fx_table[1]) ;
        }
        else if("JPY" == to_currency)
        {
            converted_amount = ((amount*fx_table[4])/fx_table[1]);
        }
        else
        {
            cout<<"Error!!"<<endl;
        }
    }
    else if("USD" == from_currency)
    {
        /* from USD */
        if("EUR" == to_currency)
        {
            converted_amount =amount / fx_table[1];
        }
        else if("USD" == to_currency)
        {
            converted_amount = amount ;
        }
        else if("SAR" == to_currency)
        {
            converted_amount =amount * fx_table[2];
        }
        else if("EGP" == to_currency)
        {
            converted_amount =amount * fx_table[3];
        }
        else if("JPY" == to_currency)
        {
            converted_amount =amount * fx_table[4];
        }
        else
        {
            cout<<"Error!!"<<endl;
        }
    }
    /* from SAR */
    else if("SAR" == from_currency)
    {
        if("EUR" == to_currency)
        {
            converted_amount = ((amount*fx_table[1])/fx_table[2]);
        }
        else if("USD" == to_currency)
        {
            converted_amount = ((amount*fx_table[0])/fx_table[2]) ;
        }
        else if("SAR" == to_currency)
        {
            converted_amount = amount;
        }
        else if("EGP" == to_currency)
        {
            converted_amount = ((amount*fx_table[3])/fx_table[2]) ;
        }
        else if("JPY" == to_currency)
        {
            converted_amount = ((amount*fx_table[4])/fx_table[2]);
        }
        else
        {
            cout<<"Error!!"<<endl;
        }
    }
        /* From EGP */
        else if("EGP" == from_currency)
    {
        if("EUR" == to_currency)
        {
            converted_amount = ((amount*fx_table[1])/fx_table[3]);
        }
        else if("USD" == to_currency)
        {
            converted_amount = ((amount*fx_table[0])/fx_table[3]) ;
        }
        else if("SAR" == to_currency)
        {
            converted_amount = ((amount*fx_table[2])/fx_table[3]);
        }
        else if("EGP" == to_currency)
        {
            converted_amount = amount  ;
        }
        else if("JPY" == to_currency)
        {
            converted_amount = ((amount*fx_table[4])/fx_table[3]);
        }
        else
        {
            cout<<"Error!!"<<endl;
        }
    }
        /* From JPY*/
        else if("JPY" == from_currency)
    {
        if("EUR" == to_currency)
        {
            converted_amount = ((amount*fx_table[1])/fx_table[4]);
        }
        else if("USD" == to_currency)
        {
            converted_amount = ((amount*fx_table[0])/fx_table[4]) ;
        }
        else if("SAR" == to_currency)
        {
            converted_amount = ((amount*fx_table[2])/fx_table[4]);
        }
        else if("EGP" == to_currency)
        {
            converted_amount = ((amount*fx_table[3])/fx_table[4]) ;
        }
        else if("JPY" == to_currency)
        {
            converted_amount = amount;
        }
        else
        {
            cout<<"Error!!"<<endl;
        }   
    }
     if('U' == rounding_policy)
    {
        /* Half UP */
        converted_amount = round(converted_amount);
    }
    else if('E' == rounding_policy )
    {
        /* Half Even */
        converted_amount = nearbyint(converted_amount);
    }
    else{/* NO Thing */}
    return converted_amount;
}

/**********************  4. Utilities  ********************/
int percent_of(int amount, double rate, char rounding_policy)
{
    double portion = 0;
    portion = amount * rate;
    if('U' == rounding_policy)
    {
        /* Half UP */
        portion = round(portion);
    }
    else if('E' == rounding_policy )
    {
        /* Half Even */
        portion = nearbyint(portion);
    }
    else{/* Nothing */}

    return (int)portion;
}

bool validate_rate(float rate)
{
    bool ret_val;
    if((0<=rate)&&(1>=rate))
    {
        ret_val = true;
    }
    else
    {
        ret_val = false;
    }
    return ret_val;
}

bool validate_non_negative(signed int amount)
{
    bool ret_val;
    if(0 <= amount)
    {
        ret_val = true;
    }
    else
    {
        ret_val = false;
    }
    return ret_val;
}





