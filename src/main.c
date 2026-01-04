#include <stdio.h>
#include <stdlib.h>
#include "../include/financials.h"

// Специално за Windows: Добавяме библиотека за кирилицата
#ifdef _WIN32
#include <windows.h>
#endif

/* * КОНСТАНТИ ЗА ЧЕСТОТА (Математически точни) */
#define FREQ_DAILY      (365.25 / 12.0)       // ~30.43 пъти/месец
#define FREQ_WORKDAYS   (5.0 * 52.177 / 12.0) // ~21.74 пъти/месец
#define FREQ_WEEKENDS   (2.0 * 52.177 / 12.0) // ~8.69 пъти/месец
#define FREQ_WEEKLY     (52.177 / 12.0)       // ~4.34 пъти/месец
#define FREQ_BIWEEKLY   (FREQ_WEEKLY / 2.0)   // ~2.17 пъти/месец
#define FREQ_MONTHLY    1.0

/* * ПОМОЩНА ФУНКЦИЯ: Изчистване на буфера */
void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* * ВАЛИДАЦИЯ: Вход на положително дробно число */
double get_positive_double(const char* prompt) {
    double value;
    int valid = 0;

    while (!valid) {
        printf("%s", prompt);
        if (scanf("%lf", &value) == 1) {
            if (value < 0) {
                printf("Грешка: Сумата не може да бъде отрицателна. Опитайте пак.\n");
            } else {
                valid = 1;
            }
        } else {
            printf("Грешка: Невалиден вход. Моля, въведете число.\n");
            clear_input_buffer();
        }
    }
    return value;
}

/* * ВАЛИДАЦИЯ: Вход на положително цяло число */
int get_positive_int(const char* prompt) {
    int value;
    int valid = 0;

    while (!valid) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            if (value <= 0) {
                printf("Грешка: Периодът трябва да е по-голям от 0.\n");
            } else {
                valid = 1;
            }
        } else {
            printf("Грешка: Невалиден вход. Моля, въведете цяло число.\n");
            clear_input_buffer();
        }
    }
    return value;
}

/* * МЕНЮ: Избор на честота */
double get_frequency_multiplier() {
    int choice;
    printf("\n--- Колко често се повтаря този навик? ---\n");
    printf("1. Ежедневно        (Всеки ден)\n");
    printf("2. Само работни дни (Пон - Пет)\n");
    printf("3. Само уикенди     (Съб - Нед)\n");
    printf("4. Седмично         (Веднъж седмично)\n");
    printf("5. Двуседмично      (На всеки две седмици)\n");
    printf("6. Месечно          (Веднъж месечно)\n");
    
    do {
        choice = get_positive_int("Изберете опция [1-6]: ");
        if (choice < 1 || choice > 6) {
            printf("Грешка: Моля, изберете число между 1 и 6.\n");
        }
    } while (choice < 1 || choice > 6);

    switch (choice) {
        case 1: return FREQ_DAILY;
        case 2: return FREQ_WORKDAYS;
        case 3: return FREQ_WEEKENDS;
        case 4: return FREQ_WEEKLY;
        case 5: return FREQ_BIWEEKLY;
        case 6: return FREQ_MONTHLY;
        default: return FREQ_MONTHLY;
    }
}

/* * ГЛАВНА ПРОГРАМА */
int main() {
    // Активиране на UTF-8 (Кирилица) за Windows конзолата
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif

    InvestmentConfig config;

    printf("==========================================\n");
    printf("    КАЛКУЛАТОР НА МИЛИОНЕРСКИТЕ НАВИЦИ    \n");
    printf("==========================================\n");

    // 1. Вход: Цена на навика
    double input_dollars = get_positive_double("Въведете цена на навика (напр. 5.50): $");
    config.expense_amount_cents = dollars_to_cents(input_dollars);

    // 2. Вход: Честота
    config.frequency_per_month = get_frequency_multiplier();

    // 3. Вход: Лихвен процент
    config.annual_growth_rate = get_positive_double("\nОчаквана годишна възвръщаемост (%) [напр. 7]: ");

    printf("\n==========================================\n");
    printf("           РЕЗУЛТАТИ ОТ ПРОГНОЗАТА        \n");
    printf("==========================================\n");

    /* * ФОРМАТИРАНЕ НА ТАБЛИЦАТА
     * Използваме по-широки колони (%18s), защото българските думи са дълги.
     * %-8s  = Годините (подравнени вляво)
     * %18s  = Колоните за пари (подравнени вдясно)
     */
    printf("%-14s | %30s | %28s | %30s\n", "ГОДИНИ", "ОБЩА СТОЙНОСТ", "ВАШИТЕ ПАРИ", "БЕЗПЛАТНИ ПАРИ");
    printf("------------------------------------------------------------------------\n");

    int time_horizons[] = {10, 20, 30};
    int num_horizons = sizeof(time_horizons) / sizeof(time_horizons[0]);

    for (int i = 0; i < num_horizons; i++) {
        int years = time_horizons[i];

        // Извличане на данните
        Money total_value = calculate_compound_interest(&config, years);
        Money your_cash   = calculate_total_invested(&config, years);
        Money free_money  = total_value - your_cash;

        /* ПЕЧАТАНЕ НА РЕДА
         * %14lld осигурява място за големи числа (напр. 100,000).
         * Така десетичните запетаи (.) ще бъдат винаги една под друга.
         */
        printf("%-8d | $%14lld.%02d | $%14lld.%02d | $%14lld.%02d\n", 
               years,
               total_value / 100, (int)(total_value % 100),
               your_cash / 100,   (int)(your_cash % 100),
               free_money / 100,  (int)(free_money % 100));
    }

    printf("------------------------------------------------------------------------\n");

    // 4. Персонализиран период (Custom Period)
    char choice;
    printf("\nИскате ли да проверите конкретен период (напр. 50 г.)? (y/n): ");
    clear_input_buffer(); 
    scanf("%c", &choice);

    if (choice == 'y' || choice == 'Y') {
        int custom_years = get_positive_int("Въведете брой години: ");
        
        Money total    = calculate_compound_interest(&config, custom_years);
        Money invested = calculate_total_invested(&config, custom_years);
        Money interest = total - invested;
        
        printf("\n>>> РЕЗУЛТАТ ЗА %d ГОДИНИ:\n", custom_years);
        printf("   Обща стойност: "); print_money(total);    printf("\n");
        printf("   Вашите пари:   "); print_money(invested); printf("\n");
        printf("   Чиста лихва:   "); print_money(interest); printf(" (Печалба!)\n");
    }

    printf("\n==========================================\n");
    return 0;
}