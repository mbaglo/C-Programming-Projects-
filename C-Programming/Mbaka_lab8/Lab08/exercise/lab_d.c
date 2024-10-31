#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Timers.h"

#define REPEAT_NUM 100000000

double traditionalMult(double x) {
    return 4.4 * x * x * x * x - 3.3 * x * x * x + 2.2 * x * x - 1.1 * x + 6.0;
}

double powerFunc(double x) {
    return 4.4 * pow(x, 4) - 3.3 * pow(x, 3) + 2.2 * pow(x, 2) - 1.1 * x + 6.0;
}

double horner(double x) {
    return (((4.4 * x - 3.3) * x + 2.2) * x - 1.1) * x + 6.0;
}

int main() {
    DECLARE_TIMER(MultTimer);
    DECLARE_TIMER(PowTimer);
    DECLARE_TIMER(HornerTimer);
    DECLARE_REPEAT_VAR(repeatVar);
    
    double x = 7.7, ans;

    /* Traditional Multiplications */
    START_TIMER(MultTimer);
    BEGIN_REPEAT_TIMING(REPEAT_NUM, repeatVar);
    ans = traditionalMult(x);
    END_REPEAT_TIMING;
    STOP_TIMER(MultTimer);
    printf("Mult performance, ans %f\n", ans);
    PRINT_TIMER(MultTimer);
    PRINT_RTIMER(MultTimer, REPEAT_NUM);
    RESET_TIMER(MultTimer);

    /* Power Function */
    START_TIMER(PowTimer);
    BEGIN_REPEAT_TIMING(REPEAT_NUM, repeatVar);
    ans = powerFunc(x);
    END_REPEAT_TIMING;
    STOP_TIMER(PowTimer);
    printf("Pow performance, ans %f\n", ans);
    PRINT_TIMER(PowTimer);
    PRINT_RTIMER(PowTimer, REPEAT_NUM);
    RESET_TIMER(PowTimer);

    /* Horner's Factorization */
    START_TIMER(HornerTimer);
    BEGIN_REPEAT_TIMING(REPEAT_NUM, repeatVar);
    ans = horner(x);
    END_REPEAT_TIMING;
    STOP_TIMER(HornerTimer);
    printf("Horner's performance, ans %f\n", ans);
    PRINT_TIMER(HornerTimer);
    PRINT_RTIMER(HornerTimer, REPEAT_NUM);

    return 0;
}
