#include <stdio.h>

int main() {
    int num1, num2;
    char operator;
    int result;

    while (1) {
        printf("Enter your calculation:\n");
        
        if (scanf("%d %c%d", &num1, &operator, &num2) != 3) {
            printf("Invalid input format!\n");
            break;
        }

        switch (operator) {
            case '+':
                result = num1 + num2;
                printf("> %d %c %d = %d\n", num1, operator, num2, result);
                break;
            case '-':
                result = num1 - num2;
                printf("> %d %c %d = %d\n", num1, operator, num2, result);
                break;
            case '*':
                result = num1 * num2;
                printf("> %d %c %d = %d\n", num1, operator, num2, result);
                break;
            case '/':
                if (num2 == 0) {
                    printf("Error: Division by zero is not allowed!\n");
                    break;
                }
                result = num1 / num2;
                printf("> %d %c %d = %d\n", num1, operator, num2, result);
                break;
            case '%':
                if (num2 == 0) {
                    printf("Error: Modulus by zero is not allowed!\n");
                    break;
                }
                result = num1 % num2;
                printf("> %d %c %d = %d\n", num1, operator, num2, result);
                break;
            case '&':
                result = num1 & num2;
                printf("> %d %c %d = %d\n", num1, operator, num2, result);
                break;
            default:
                printf("Invalid calculation! \"%d %c %d\"\n", num1, operator, num2);
                return 0;
        }
    }
    return 0;
}

