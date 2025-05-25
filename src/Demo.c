#include "MPXL.h"
#include<stdio.h>

int main(int argc, char** argv)
{
    puts("\n=== MPXL BigInt Library Demo ===\n");
    
    // ==============================================
    // 1. Basic Arithmetic with Macros
    // ==============================================
    puts("[1] Arithmetic Operations:");
    BigInt a = MAKE("12345678901234567890");
    BigInt b = MAKE("9876543210");
    
    BigInt sum = ADD(&a, &b);
    printf("ADD(a, b) = ");
    printBigInt(sum);
    puts("");
    
    BigInt diff = SUB(&a, &b);
    printf("SUB(a, b) = ");
    printBigInt(diff);
    puts("");
    
    BigInt prod = MUL(&a, &b);
    printf("MUL(a, b) = ");
    printBigInt(prod);
    puts("");
    
    BigInt quot = DEVIDE(a, b);  // Note: divideBigInt doesn't use pointers
    printf("DIVIDE(a, b) = ");
    printBigInt(quot);
    puts("\n");
    
    // ==============================================
    // 2. Mathematical Functions
    // ==============================================
    puts("[2] GCD/LCM:");
    BigInt x = MAKE("42");
    BigInt y = MAKE("56");
    
    BigInt gcd_val = GCD(&x, &y);
    printf("GCD(42, 56) = ");
    printBigInt(gcd_val);
    puts("");
    
    BigInt lcm_val = LCM(&x, &y);
    printf("LCM(42, 56) = ");
    printBigInt(lcm_val);
    puts("\n");
    
    // ==============================================
    // 3. Advanced Functions
    // ==============================================
    puts("[3] Advanced Operations:");
    BigInt num = MAKE("10");
    BigInt fact = FACTORIAL(num);
    printf("10! = ");
    printBigInt(fact);
    puts("");
    
    BigInt hundred = HUNDRED();
    printf("HUNDRED() = ");
    printBigInt(hundred);
    puts("\n");
    
    // ==============================================
    // 4. Cleanup
    // ==============================================
    puts("[4] Memory Cleanup:");
    
  
    
    freeBigInt(a);
    freeBigInt(b);
    freeBigInt(sum);
    freeBigInt(diff);
    freeBigInt(prod);
    freeBigInt(quot);
    freeBigInt(x);
    freeBigInt(y);
    freeBigInt(gcd_val);
    freeBigInt(lcm_val);
    freeBigInt(num);
    freeBigInt(fact);
    freeBigInt(hundred);
    
    puts("=== Demo Complete ===");
}
