#include <stdio.h>  // Standard library for input/output




int main(void) {
    // Print a message to the console
    puts("This is the numerical Chapter!");
    fflush(stdout);  // Flush the output buffer to ensure the message is printed

    // --- Part 1: Multiply two 16-bit fixed-point numbers (0.5 * 0.5) ---

    // Define two 16-bit fixed-point numbers
    int num1_fixed = (int)(0.5 * 32768); // Fixed-point representation of 0.5
    int num2_fixed = (int)(0.5 * 32768); // Fixed-point representation of 0.5

    // Multiply the two fixed-point numbers and store the result as a 32-bit number
    long result_fixed = (long)num1_fixed * (long)num2_fixed;

    // Shift the result to adjust for the fixed-point format scaling (divide by 32768)
    result_fixed = result_fixed >> 15;

    // Print the result of the fixed-point multiplication
    printf("Fixed-point multiplication result (0.5 * 0.5) = %lx (fixed-point in Hex)\n", result_fixed);

    // --- Part 2: Multiply two floating-point numbers (0.5 * 0.5) ---

    // Define two floating-point numbers
    float num1_float = 0.5;
    float num2_float = 0.5;

    // Multiply the two floating-point numbers
    float result_float = num1_float * num2_float;

    // Print the result of the floating-point multiplication
   printf("Floating-point multiplication result (0.5 * 0.5) = %.2f (floating-point)\n", result_float);

    return 0;  // End of the program
}


