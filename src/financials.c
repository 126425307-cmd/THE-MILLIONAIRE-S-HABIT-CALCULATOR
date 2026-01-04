#include <stdio.h>      
#include <math.h>    
#include "../include/financials.h" //  Header файл

/* * ------------------------------------------------------------------
 * ПОМОЩНИ ФУНКЦИИ ЗА ПАРИЧНИ СТОЙНОСТИ (Money Utility Functions)
 * ------------------------------------------------------------------
 */

/**
 * Преобразува долари (дробно число) в центове (цяло число).
 * Използваме закръгляне (+0.5), за да избегнем грешки при плаваща запетая.
 * Пример: 10.50 -> 1050
 */
Money dollars_to_cents(double dollars) {
    return (Money)(dollars * 100.0 + 0.5);
}

/**
 * Форматира и отпечатва парична сума директно в конзолата.
 */
void print_money(Money amount) {
    long long dollars = amount / 100;    
    int cents = (int)(amount % 100);    

    printf("$%lld.%02d", dollars, cents);
}

/* * ------------------------------------------------------------------
 * ОСНОВНА ЛОГИКА ЗА ФИНАНСОВИ ИЗЧИСЛЕНИЯ
 * ------------------------------------------------------------------
 */

/**
 * Изчислява бъдещата стойност на инвестицията чрез сложна лихва.
 * Формула: FV = P * [ (1 + r)^n - 1 ] / r
 */
Money calculate_compound_interest(const InvestmentConfig *config, int years) {
    // 1. ПОДГОТОВКА НА ВХОДА:
    // Превръщаме центовете (int) обратно в долари (double) за математическите сметки.
    // Умножаваме по честотата, за да получим месечната вноска.
    double monthly_contribution = (double)config->expense_amount_cents * config->frequency_per_month;

    // 2. ПОДГОТОВКА НА ЛИХВИТЕ:
    // Превръщаме годишния процент (напр. 7.0) в десетичен вид (0.07).
    double rate_decimal = config->annual_growth_rate / 100.0;
    
    // Пресмятаме месечния лихвен процент (r).
    double monthly_rate = rate_decimal / 12.0;

    // Пресмятаме общия брой месеци (n).
    int total_months = years * 12;

    // 3. СПЕЦИАЛЕН СЛУЧАЙ: 0% Лихва
    // Ако лихвата е 0, формулата би делила на нула (грешка).
    // В този случай просто умножаваме вноската по месеците.
    if (monthly_rate == 0) {
        return (Money)(monthly_contribution * total_months);
    }

    // 4. ИЗЧИСЛЕНИЕ ПО ФОРМУЛАТА:
    // Стъпка А: Растежният фактор (1 + r)^n
    double compound_factor = pow(1.0 + monthly_rate, total_months);

    // Стъпка В: Цялата формула
    double future_value_double = monthly_contribution * (compound_factor - 1.0) / monthly_rate;

    // 5. РЕЗУЛТАТ:
    // Закръгляме обратно към най-близкия цент и връщаме като Money.
    return (Money)(future_value_double + 0.5);
}

/**
 * Изчислява само сумата, която потребителят реално е "извадил от джоба си" (Главница).
 * Това не включва никаква лихва.
 * Формула: Месечна вноска * Общ брой месеци
 */
Money calculate_total_invested(const InvestmentConfig *config, int years) {
    // Изчисляваме месечната вноска (същата логика като горе)
    double monthly_contribution = (double)config->expense_amount_cents * config->frequency_per_month;

    int total_months = years * 12;
    double total_invested = monthly_contribution * total_months;

    // Връщаме като Money (центове)
    return (Money)(total_invested + 0.5);
}