#include "ternary.h" // Include the function declaration
#include "stdio.h"

int main(void) {
    int cond = 1;
    int if_true = 10;
    int if_false = 5;
    int ternary_result = cond ? if_true : if_false;

    if (ternary(cond, if_true, if_false) == ternary_result) {
        printf("Test passed!\n");
    } else {
        printf("Test failed!\n");
    }

    return 0;
}
