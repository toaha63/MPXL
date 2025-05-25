MPXL - Big Integer Library for C

Introduction:
A C library for arbitrary-precision integer arithmetic. Handles integers of any size limited only by available memory.

Uses:
- Mathematical computations
- Financial calculations
- Scientific computing

Features:
- Basic arithmetic operations (+, -, *, /, %)
- Bitwise operations
- Number theory functions
- Memory efficient
- Easy-to-use API

FUNCTIONS:

1) BigInt MAKE(char* a)  
   -> Creates a new BigInt from string

2) BigInt createBigIntFromInt(long long value)  
   -> Creates BigInt from integer value

3) BigInt copyBigInt(BigInt num)  
   -> Returns a copy of given BigInt

4) void freeBigInt(BigInt num)  
   -> Frees memory allocated for BigInt

5) BigInt addBigInt(BigInt a, BigInt b)  
   -> Returns a + b

6) BigInt subtractBigInt(BigInt a, BigInt b)  
   -> Returns a - b

7) BigInt multiplyBigInt(BigInt a, BigInt b)  
   -> Returns a * b

8) BigInt divideBigInt(BigInt a, BigInt b)  
   -> Returns a / b

9) BigInt modBigInt(BigInt a, BigInt b)  
   -> Returns a % b

10) BigInt absBigInt(BigInt num)  
    -> Returns absolute value

11) BigInt negateBigInt(BigInt num)  
    -> Returns negated value

12) int compareBigInt(BigInt a, BigInt b)  
    -> Returns 1 if a>b, -1 if a<b, 0 if equal

13) bool isPrime(BigInt num)  
    -> Checks if number is prime

14) BigInt gcdBigInt(BigInt a, BigInt b)  
    -> Returns greatest common divisor

15) BigInt lcmBigInt(BigInt a, BigInt b)  
    -> Returns least common multiple

16) BigInt BigFactorial(BigInt num)  
    -> Returns factorial of num

17) BigInt BigPower(BigInt base, BigInt exp)  
    -> Returns base^exp

18) BigInt leftShiftBigInt(BigInt num, int shift)  
    -> Left shifts number by 'shift' bits

19) BigInt rightShiftBigInt(BigInt num, int shift)  
    -> Right shifts number by 'shift' bits

20) char* bigIntToString(BigInt num)  
    -> Converts BigInt to string

21) void printBigInt(BigInt num)  
    -> Prints BigInt to stdout

MACROS:

1) ADD(a, b, ...)  
   -> Adds multiple BigInts (NULL terminated)

2) SUB(a, b, ...)  
   -> Subtracts multiple BigInts

3) MUL(a, b, ...)  
   -> Multiplies multiple BigInts

4) DIVIDE(a, b)  
   -> Divides two BigInts

5) MAX(a, b, ...)  
   -> Returns maximum of given BigInts

6) MIN(a, b, ...)  
   -> Returns minimum of given BigInts

Example Usage:
```C
#include "MPXL.h"

int main()
{
    BigInt a = MAKE("123456789");
    BigInt b = MAKE("6E700");
    
    BigInt sum = ADD(&a, &b);
    printBigInt(sum);
    
    freeBigInt(a);
    freeBigInt(b);
    freeBigInt(sum);
}
```
Notes:
- Always free BigInt objects after use
- NULL terminate variable argument lists
- Check for memory allocation errors
- Functions return new BigInt objects that must be freed