#ifndef FINANCIALS_H
#define FINANCIALS_H

typedef long long Money;
typedef struct
{
     Money expense_amount_cents;
    double frequency_per_month;
    double annual_growth_rate;
} InvestmentConfig;

Money calculate_compound_interest(const InvestmentConfig *config, int years);
Money calculate_total_invested(const InvestmentConfig *config, int years);
Money dollars_to_cents(double dollars);
void print_money(Money amount_cents);
#endif // FINANCIALS_H