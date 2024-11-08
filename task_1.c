
#include <stdint.h>
#include <stdio.h>

int16_t int16_a = 16384;
int16_t int16_b = 16384;

uint16_t uint16_a = 16384;
uint16_t uint16_b = 16384;

float float_a = 0.50;
float float_b = 0.50;


// function to display floating-point values
void display_float(float value) {
    int int_part = (int)value;                         // Integer part
    int decimal_part = (int)((value - int_part) * 100); // Decimal part (2 decimal places)
    if (decimal_part < 0) decimal_part *= -1;          // Handle negative decimal values
    printf("%d.%02d", int_part, decimal_part);
}

int main(void)
{
    // 1. Multiply two 16-bit signed integers
    int64_t int_result = (int64_t)int16_a * int16_b;
    printf("Multiplication of int: %d * %d = %lld\n", int16_a, int16_b, int_result);

    // 2. Multiply two 16-bit unsigned integers
    uint64_t uint_result = (uint64_t)uint16_a * uint16_b;
    printf("Multiplication of unsigned int: %u * %u = %llu\n", uint16_a, uint16_b, uint_result);

    // 3. Multiply two 32-bit floating-point numbers
    float float_result = float_a * float_b;
    printf("Multiplication of float: ");
    display_float(float_a);
    printf(" * ");
    display_float(float_b);
    printf(" = ");
    display_float(float_result);
    printf("\n");

    return 0;
}
