#ifndef MPXL_H
#define MPXL_H

#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<limits.h>

// BigInt structure definition
typedef struct BigInt
{

    int* digits;
    int length;
    int sign;

}BigInt;


//Macro area
#define MAKE(numStr) createBigInt(numStr)
#define ADD(...) addMultipleBigInts(__VA_ARGS__, NULL)
#define SUB(...) subtractMultipleBigInts(__VA_ARGS__, NULL)
#define MUL(...) mulMultipleBigInts(__VA_ARGS__, NULL)
#define DEVIDE(a , b) divideBigInt( a,  b)
#define GCD(...) gcdMultiple(__VA_ARGS__, NULL)
#define LCM(...) lcmMultiple(__VA_ARGS__, NULL)
#define MAX(...) *maxMultiple(__VA_ARGS__, NULL)
#define MIN(...) *minMultiple(__VA_ARGS__, NULL)
#define FACTORIAL(num) BigFactorial(num)
#define HUNDRED() Hundred()
#define THOUSAND() Thousand()

#define new /* Just for fun 😂. User can use it like the "new" keyword like Java */  


// Function prototype area
void removeLeadingZeros(BigInt* num);
int* toBinary(BigInt num, int* bitLength);
char* bigIntToString(BigInt num);
BigInt fromBinary(int* bits, int bitLength);
BigInt createBigInt(const char* numStr);
BigInt createBigIntFromInt(long long value);
BigInt copyBigInt(BigInt num);
void freeBigInt(BigInt num);
int compareBigInt(BigInt a, BigInt b);
bool isNegative(BigInt num);
bool isZero(BigInt num);
BigInt absBigInt(BigInt num);
BigInt negateBigInt(BigInt num);
BigInt subtractBigInt(BigInt a, BigInt b);
BigInt addBigInt(BigInt a, BigInt b);
BigInt addMultipleBigInts(BigInt* first, ...);
BigInt subtractMultipleBigInts(BigInt* first, ...);
BigInt mulMultipleBigInts(BigInt* first, ...);
BigInt* maxMultiple(BigInt* first, ...);
BigInt* minMultiple(BigInt* first, ...);
BigInt gcdMultiple(BigInt* first, ...);
BigInt lcmMultiple(BigInt* first, ...);
BigInt multiplyBigInt(BigInt a, BigInt b);
BigInt divideBigInt(BigInt a, BigInt b);
BigInt modBigInt(BigInt a, BigInt b);
BigInt nextPrime(BigInt num);
BigInt leftShiftBigInt(BigInt num, int shift);
BigInt rightShiftBigInt(BigInt num, int shift);
BigInt andBigInt(BigInt a, BigInt b);
BigInt orBigInt(BigInt a, BigInt b);
BigInt Hundred(void);
BigInt Thousand(void);
BigInt gcdBigInt(BigInt a, BigInt b);
BigInt lcmBigInt(BigInt a, BigInt b);
void printBigInt(BigInt num);
BigInt BigFactorial(BigInt num);
BigInt BigPower(BigInt base, BigInt exponent);
bool isEven(BigInt num);
bool isOdd(BigInt num);
bool isPalindrome(BigInt num);
bool isPrime(BigInt num);
bool isBig(BigInt a, BigInt b);
bool isSmall(BigInt a, BigInt b);
bool isEqual(BigInt a, BigInt b);
bool isPerfectSquare(BigInt num);
bool isDivisible(BigInt a, BigInt b);
bool isNthPerfectRoot(BigInt num, BigInt n);
bool isPerfectCubicNumber(BigInt num);


#endif
