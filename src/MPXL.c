#include "MPXL.h"


void removeLeadingZeros(BigInt* num)
{
    while (num->length > 1 && num->digits[num->length - 1] == 0)
    {
        num->length--;
    }
    if (num->length == 1 && num->digits[0] == 0)
    {
        num->sign = 1;
    }
}
/*
BigInt createBigInt(const char* numStr)
{
    BigInt num;
    int start = 0;
    
    if (numStr[0] == '-')
    {
        num.sign = -1;
        start = 1;
    }
    else
    {
        num.sign = 1;
    }
    
    num.length = strlen(numStr) - start;
    num.digits = (int*)calloc(num.length, sizeof(int));
    
    for (int i = 0; i < num.length; i++)
    {
        if (numStr[start + num.length - 1 - i] < '0' || numStr[start + num.length - 1 - i] > '9')
        {
            fprintf(stderr, "Invalid character in number string\n");
            exit(EXIT_FAILURE);
        }
        num.digits[i] = numStr[start + num.length - 1 - i] - '0';
    }
    
    removeLeadingZeros(&num);
    return num;
}
*/

BigInt createBigInt(const char* numStr) 
{
    BigInt num;
    int start = 0;
    
    // Handle sign
    if (numStr[0] == '-') 
    {
        num.sign = -1;
        start = 1;
    } 
    else 
    {
        num.sign = 1;
    }
    
    // Check for scientific notation
    char* ePos = strchr(numStr, 'E');
    if (!ePos) 
    {
        ePos = strchr(numStr, 'e');
    }
    
    if (ePos) 
    {
        // Handle scientific notation (e.g., "6E100" or "6E-100")
        char* mantissaStr = (char*)calloc(ePos - (numStr + start) + 1, sizeof(char));
        if (!mantissaStr) 
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strncpy(mantissaStr, numStr + start, ePos - (numStr + start));
        mantissaStr[ePos - (numStr + start)] = '\0';
        
        char* exponentStr = ePos + 1;
        BigInt mantissa = createBigInt(mantissaStr);
        BigInt exponent = createBigInt(exponentStr);
        
        // Calculate 10^exponent
        BigInt ten = MAKE("10");
        BigInt power = BigPower(ten, exponent);
        
        // Multiply mantissa by 10^exponent
        BigInt result = multiplyBigInt(mantissa, power);
        result.sign = num.sign; // Preserve original sign
        
        freeBigInt(mantissa);
        freeBigInt(exponent);
        freeBigInt(ten);
        freeBigInt(power);
        free(mantissaStr);
        
        return result;
    }
    
    // Regular number processing
    num.length = strlen(numStr) - start;
    num.digits = (int*)calloc(num.length, sizeof(int));
    if (!num.digits) 
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < num.length; i++) 
    {
        if (numStr[start + num.length - 1 - i] < '0' || numStr[start + num.length - 1 - i] > '9') 
        {
            fprintf(stderr, "Invalid character in number string\n");
            free(num.digits);
            exit(EXIT_FAILURE);
        }
        num.digits[i] = numStr[start + num.length - 1 - i] - '0';
    }
    
    removeLeadingZeros(&num);
    return num;
}

BigInt createBigIntFromInt(long long value)
{
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%lld", value);
    return MAKE(buffer);
}

BigInt copyBigInt(BigInt num)
{
    BigInt copy;
    copy.length = num.length;
    copy.sign = num.sign;
    copy.digits = (int*)calloc(copy.length, sizeof(int));
    memcpy(copy.digits, num.digits, copy.length * sizeof(int));
    return copy;
}

void freeBigInt(BigInt num)
{
    if (num.digits != NULL)
    {
        memset(num.digits, 0, num.length * sizeof(int));
        free(num.digits);
    }
}

int compareBigInt(BigInt a, BigInt b)
{
    if (a.sign != b.sign)
    {
        return a.sign > b.sign ? 1 : -1;
    }
    
    if (a.length != b.length)
    {
        return (a.length > b.length) ? a.sign : -a.sign;
    }
    
    for (int i = a.length - 1; i >= 0; i--)
    {
        if (a.digits[i] != b.digits[i])
        {
            return (a.digits[i] > b.digits[i]) ? a.sign : -a.sign;
        }
    }
    
    return 0;
}

bool isNegative(BigInt num)
{
    return num.sign == -1;
}

bool isZero(BigInt num)
{
    return num.length == 1 && num.digits[0] == 0;
}

BigInt absBigInt(BigInt num)
{
    BigInt result = copyBigInt(num);
    result.sign = 1;
    return result;
}

BigInt negateBigInt(BigInt num)
{
    BigInt result = copyBigInt(num);
    result.sign *= -1;
    return result;
}

BigInt subtractBigInt(BigInt a, BigInt b)
{
    if (a.sign != b.sign)
    {
        BigInt negB = negateBigInt(b);
        BigInt result = addBigInt(a, negB);
        freeBigInt(negB);
        return result;
    }
    
    BigInt absA = absBigInt(a);
    BigInt absB = absBigInt(b);
    
    if (compareBigInt(absA, absB) < 0)
    {
        BigInt result = subtractBigInt(b, a);
        result.sign = -a.sign;
        freeBigInt(absA);
        freeBigInt(absB);
        return result;
    }
    
    BigInt result;
    result.digits = (int*)calloc(a.length, sizeof(int));
    result.length = 0;
    result.sign = a.sign;
    
    int borrow = 0;
    for (int i = 0; i < a.length; i++)
    {
        int digitA = a.digits[i];
        int digitB = (i < b.length) ? b.digits[i] : 0;
        
        int diff = digitA - digitB - borrow;
        if (diff < 0)
        {
            diff += 10;
            borrow = 1;
        }
        else
        {
            borrow = 0;
        }
        
        result.digits[result.length++] = diff;
    }
    
    removeLeadingZeros(&result);
    freeBigInt(absA);
    freeBigInt(absB);
    return result;
}

BigInt addBigInt(BigInt a, BigInt b)
{
    if (a.sign != b.sign)
    {
        if (a.sign == -1)
        {
            BigInt absA = absBigInt(a);
            BigInt result = subtractBigInt(b, absA);
            freeBigInt(absA);
            return result;
        }
        else
        {
            BigInt absB = absBigInt(b);
            BigInt result = subtractBigInt(a, absB);
            freeBigInt(absB);
            return result;
        }
    }
    
    BigInt result;
    int maxLength = (a.length > b.length) ? a.length : b.length;
    result.digits = (int*)calloc(maxLength + 1, sizeof(int));
    result.length = 0;
    result.sign = a.sign;
    
    int carry = 0;
    for (int i = 0; i < maxLength || carry; i++)
    {
        int digitA = (i < a.length) ? a.digits[i] : 0;
        int digitB = (i < b.length) ? b.digits[i] : 0;
        
        int sum = digitA + digitB + carry;
        result.digits[result.length++] = sum % 10;
        carry = sum / 10;
    }
    
    return result;
}

BigInt multiplyBigInt(BigInt a, BigInt b)
{
    BigInt result;
    result.length = a.length + b.length;
    result.digits = (int*)calloc(result.length, sizeof(int));
    result.sign = a.sign * b.sign;
    
    for (int i = 0; i < a.length; i++)
    {
        int carry = 0;
        for (int j = 0; j < b.length || carry; j++)
        {
            int digitB = (j < b.length) ? b.digits[j] : 0;
            int product = result.digits[i + j] + a.digits[i] * digitB + carry;
            result.digits[i + j] = product % 10;
            carry = product / 10;
        }
    }
    
    removeLeadingZeros(&result);
    return result;
}

BigInt divideBigInt(BigInt a, BigInt b)
{
    if (isZero(b))
    {
        fprintf(stderr, "Division by zero\n");
        exit(EXIT_FAILURE);
    }
    
    BigInt zero = MAKE("0");
    BigInt one = MAKE("1");
    BigInt ten = MAKE("10");
    
    int resultSign = a.sign * b.sign;
    BigInt absA = absBigInt(a);
    BigInt absB = absBigInt(b);
    
    if (compareBigInt(absA, absB) < 0)
    {
        freeBigInt(absA);
        freeBigInt(absB);
        freeBigInt(one);
        freeBigInt(ten);
        return zero;
    }
    
    BigInt result = MAKE("0");
    BigInt current = MAKE("0");
    
    for (int i = absA.length - 1; i >= 0; i--)
    {
        BigInt temp = multiplyBigInt(current, ten);
        freeBigInt(current);
        current = temp;
        
        BigInt digit = createBigIntFromInt(absA.digits[i]);
        temp = addBigInt(current, digit);
        freeBigInt(current);
        freeBigInt(digit);
        current = temp;
        
        int count = 0;
        BigInt testVal = copyBigInt(absB);
        while (compareBigInt(testVal, current) <= 0)
        {
            count++;
            temp = addBigInt(testVal, absB);
            freeBigInt(testVal);
            testVal = temp;
        }
        freeBigInt(testVal);
        
        temp = multiplyBigInt(result, ten);
        freeBigInt(result);
        result = temp;
        
        digit = createBigIntFromInt(count);
        temp = addBigInt(result, digit);
        freeBigInt(result);
        freeBigInt(digit);
        result = temp;
        
        BigInt countBig = createBigIntFromInt(count);
        BigInt product = multiplyBigInt(absB, countBig);
        temp = subtractBigInt(current, product);
        freeBigInt(current);
        freeBigInt(product);
        freeBigInt(countBig);
        current = temp;
    }
    
    result.sign = resultSign;
    removeLeadingZeros(&result);
    
    freeBigInt(zero);
    freeBigInt(one);
    freeBigInt(ten);
    freeBigInt(current);
    freeBigInt(absA);
    freeBigInt(absB);
    
    return result;
}

BigInt modBigInt(BigInt a, BigInt b)
{
    BigInt div = divideBigInt(a, b);
    BigInt product = multiplyBigInt(div, b);
    BigInt result = subtractBigInt(a, product);
    
    freeBigInt(div);
    freeBigInt(product);
    
    return result;
}

BigInt leftShiftBigInt(BigInt num, int shift)
{
    BigInt result = copyBigInt(num);
    BigInt two = MAKE("2");
    
    for (int i = 0; i < shift; i++)
    {
        BigInt temp = multiplyBigInt(result, two);
        freeBigInt(result);
        result = temp;
    }
    
    freeBigInt(two);
    return result;
}

BigInt rightShiftBigInt(BigInt num, int shift)
{
    BigInt result = copyBigInt(num);
    BigInt two = MAKE("2");
    
    for (int i = 0; i < shift; i++)
    {
        BigInt temp = divideBigInt(result, two);
        freeBigInt(result);
        result = temp;
    }
    
    freeBigInt(two);
    return result;
}

// Convert BigInt to binary representation (array of bits)
int* toBinary(BigInt num, int* bitLength)
{
    if (isZero(num))
    {
        *bitLength = 1;
        int* bits = (int*)calloc(1, sizeof(int));
        bits[0] = 0;
        return bits;
    }
    
    // Estimate maximum bits needed (log2(10^digits) ≈ 3.32 * digits)
    int maxBits = num.length * 4 + 10;
    int* bits = (int*)calloc(maxBits, sizeof(int));
    *bitLength = 0;
    
    BigInt temp = copyBigInt(num);
    BigInt two = MAKE("2");
    
    while (!isZero(temp))
    {
        BigInt mod = modBigInt(temp, two);
        bits[*bitLength] = mod.digits[0];
        (*bitLength)++;
        
        BigInt div = divideBigInt(temp, two);
        freeBigInt(temp);
        freeBigInt(mod);
        temp = div;
    }
    
    freeBigInt(temp);
    freeBigInt(two);
    return bits;
}

// Convert binary array back to BigInt
BigInt fromBinary(int* bits, int bitLength)
{
    BigInt result = MAKE("0");
    BigInt power = MAKE("1");
    BigInt two = MAKE("2");
    
    for (int i = 0; i < bitLength; i++)
    {
        if (bits[i])
        {
            BigInt temp = addBigInt(result, power);
            freeBigInt(result);
            result = temp;
        }
        
        BigInt temp = multiplyBigInt(power, two);
        freeBigInt(power);
        power = temp;
    }
    
    freeBigInt(power);
    freeBigInt(two);
    return result;
}

BigInt andBigInt(BigInt a, BigInt b)
{
    int bitsA_len, bitsB_len;
    int* bitsA = toBinary(a, &bitsA_len);
    int* bitsB = toBinary(b, &bitsB_len);
    
    int minLength = (bitsA_len < bitsB_len) ? bitsA_len : bitsB_len;
    int* resultBits = (int*)calloc(minLength, sizeof(int));
    
    for (int i = 0; i < minLength; i++)
    {
        resultBits[i] = bitsA[i] & bitsB[i];
    }
    
    BigInt result = fromBinary(resultBits, minLength);
    
    free(bitsA);
    free(bitsB);
    free(resultBits);
    
    return result;
}

BigInt orBigInt(BigInt a, BigInt b)
{
    int bitsA_len, bitsB_len;
    int* bitsA = toBinary(a, &bitsA_len);
    int* bitsB = toBinary(b, &bitsB_len);
    
    int maxLength = (bitsA_len > bitsB_len) ? bitsA_len : bitsB_len;
    int* resultBits = (int*)calloc(maxLength, sizeof(int));
    
    for (int i = 0; i < maxLength; i++)
    {
        int bitA = (i < bitsA_len) ? bitsA[i] : 0;
        int bitB = (i < bitsB_len) ? bitsB[i] : 0;
        resultBits[i] = bitA | bitB;
    }
    
    BigInt result = fromBinary(resultBits, maxLength);
    
    free(bitsA);
    free(bitsB);
    free(resultBits);
    
    return result;
}

BigInt Hundred(void)
{
    return MAKE("100");
}
BigInt Thousand(void)
{
    return MAKE("1000");
}

BigInt gcdBigInt(BigInt a, BigInt b)
{
    BigInt zero = MAKE("0");
    BigInt absA = absBigInt(a);
    BigInt absB = absBigInt(b);
    
    while (!isZero(absB))
    {
        BigInt temp = modBigInt(absA, absB);
        freeBigInt(absA);
        absA = absB;
        absB = temp;
    }
    
    freeBigInt(zero);
    freeBigInt(absB);
    return absA;
}

BigInt lcmBigInt(BigInt a, BigInt b)
{
    BigInt gcd = gcdBigInt(a, b);
    BigInt product = multiplyBigInt(a, b);
    BigInt result = divideBigInt(product, gcd);
    
    freeBigInt(gcd);
    freeBigInt(product);
    
    return result;
}

BigInt BigFactorial(BigInt num)
{
    if (isNegative(num))
    {
        fprintf(stderr, "Factorial of negative number is undefined in mathmetics\n");
        exit(EXIT_FAILURE);
    }

    BigInt result = MAKE("1");
    BigInt one = MAKE("1");
    BigInt i = MAKE("2");

    if (isZero(num))
    {
        freeBigInt(one);
        freeBigInt(i);
        return result;
    }

    BigInt limit = addBigInt(num, one);
    
    while (compareBigInt(i, limit) < 0)
    {
        BigInt temp = multiplyBigInt(result, i);
        freeBigInt(result);
        result = temp;
        
        temp = addBigInt(i, one);
        freeBigInt(i);
        i = temp;
    }

    freeBigInt(one);
    freeBigInt(i);
    freeBigInt(limit);
    return result;
}

BigInt BigPower(BigInt base, BigInt exponent)
{
    if (isNegative(exponent))
    {
        fprintf(stderr, "Negative exponents not supported in this version.\n");
        exit(EXIT_FAILURE);
    }

    BigInt zero = MAKE("0");
    BigInt one = MAKE("1");
    BigInt two = MAKE("2");
    
    BigInt result = MAKE("1");
    BigInt currentBase = copyBigInt(base);
    BigInt currentExponent = copyBigInt(exponent);
    
    if(isZero(currentExponent) || isEqual(one, currentExponent))
    {
        freeBigInt(zero);
        freeBigInt(one);
        freeBigInt(two);
        freeBigInt(currentBase);
        freeBigInt(currentExponent);
        return one;
    }
    

    while (!isZero(currentExponent))
    {
        BigInt mod = modBigInt(currentExponent, two);
        if (isEqual(mod, one))
        {
            BigInt temp = multiplyBigInt(result, currentBase);
            freeBigInt(result);
            result = temp;
        }
        
        BigInt temp = multiplyBigInt(currentBase, currentBase);
        freeBigInt(currentBase);
        currentBase = temp;
        
        temp = divideBigInt(currentExponent, two);
        freeBigInt(currentExponent);
        currentExponent = temp;
        
        freeBigInt(mod);
    }

    freeBigInt(zero);
    freeBigInt(one);
    freeBigInt(two);
    freeBigInt(currentBase);
    freeBigInt(currentExponent);
    return result;
}

bool isEven(BigInt num)
{
    if (isZero(num)) return true;
    BigInt two = MAKE("2");
    BigInt mod = modBigInt(num, two);
    bool result = isZero(mod);
    freeBigInt(two);
    freeBigInt(mod);
    return result;
}

bool isOdd(BigInt num)
{
    return !isEven(num);
}

bool isPalindrome(BigInt num)
{
    for (int i = 0; i < num.length / 2; i++)
    {
        if (num.digits[i] != num.digits[num.length - 1 - i])
        {
            return false;
        }
    }
    return true;
}

bool isPrime(BigInt num)
{
    BigInt absNum = absBigInt(num);
    BigInt zero = MAKE("0");
    BigInt one = MAKE("1");
    BigInt two = MAKE("2");
    
    if (isZero(absNum) || isEqual(absNum, one)) 
    {
        freeBigInt(absNum);
        freeBigInt(zero);
        freeBigInt(one);
        freeBigInt(two);
        return false;
    }
    if (isEqual(absNum, two)) 
    {
        freeBigInt(absNum);
        freeBigInt(zero);
        freeBigInt(one);
        freeBigInt(two);
        return true;
    }
    if (isEven(absNum)) 
    {
        freeBigInt(absNum);
        freeBigInt(zero);
        freeBigInt(one);
        freeBigInt(two);
        return false;
    }

    BigInt i = MAKE("3");
    // Simple approximation for square root - just check up to absNum/2
    BigInt sqrtApprox = divideBigInt(absNum, two);

    while (compareBigInt(i, sqrtApprox) <= 0)
    {
        BigInt mod = modBigInt(absNum, i);
        if (isZero(mod))
        {
            freeBigInt(absNum);
            freeBigInt(zero);
            freeBigInt(one);
            freeBigInt(two);
            freeBigInt(i);
            freeBigInt(sqrtApprox);
            freeBigInt(mod);
            return false;
        }
        freeBigInt(mod);
        
        BigInt temp = addBigInt(i, two);
        freeBigInt(i);
        i = temp;
    }

    freeBigInt(absNum);
    freeBigInt(zero);
    freeBigInt(one);
    freeBigInt(two);
    freeBigInt(i);
    freeBigInt(sqrtApprox);
    return true;
}

// Fixed comparison functions - don't free parameters
bool isBig(BigInt a, BigInt b)
{
    return compareBigInt(a, b) > 0;
}

bool isSmall(BigInt a, BigInt b)
{
    return compareBigInt(a, b) < 0;
}

bool isEqual(BigInt a, BigInt b)
{
    return compareBigInt(a, b) == 0;
}

bool isPerfectSquare(BigInt num)
{
    if (isNegative(num)) return false;
    
    BigInt zero = MAKE("0");
    BigInt one = MAKE("1");
    
    if (isZero(num) || isEqual(num, one)) 
    {
        freeBigInt(zero);
        freeBigInt(one);
        return true;
    }

    BigInt low = MAKE("1");
    BigInt high = copyBigInt(num);
    BigInt two = MAKE("2");

    while (compareBigInt(low, high) <= 0)
    {
        BigInt sum = addBigInt(low, high);
        BigInt mid = divideBigInt(sum, two);
        BigInt square = multiplyBigInt(mid, mid);
        
        int cmp = compareBigInt(square, num);
        if (cmp == 0)
        {
            freeBigInt(zero);
            freeBigInt(one);
            freeBigInt(low);
            freeBigInt(high);
            freeBigInt(two);
            freeBigInt(sum);
            freeBigInt(mid);
            freeBigInt(square);
            return true;
        }
        else if (cmp < 0)
        {
            BigInt temp = addBigInt(mid, one);
            freeBigInt(low);
            low = temp;
        }
        else
        {
            BigInt temp = subtractBigInt(mid, one);
            freeBigInt(high);
            high = temp;
        }
        
        freeBigInt(sum);
        freeBigInt(mid);
        freeBigInt(square);
    }

    freeBigInt(zero);
    freeBigInt(one);
    freeBigInt(low);
    freeBigInt(high);
    freeBigInt(two);
    return false;
}


void printBigInt(BigInt num)
{
    if (num.sign == -1)
    {
        printf("-");
    }
    for (int i = (num.length - 1); i >= 0; i--)
    {
        printf("%d", num.digits[i]);
    }
}

BigInt addMultipleBigInts(BigInt* first, ...) 
{
    va_list args;
    va_start(args, first);
    
    BigInt result = copyBigInt(*first);
    
    BigInt* current;
    while ((current = va_arg(args, BigInt*)))
     {  // NULL-terminated
        BigInt temp = addBigInt(result, *current);
        freeBigInt(result);
        result = temp;
    }
    
    va_end(args);
    return result;
}

BigInt subtractMultipleBigInts(BigInt* first, ...) 
{
    va_list args;
    va_start(args, first);
    
    BigInt result = copyBigInt(*first);
    
    BigInt* current;
    while ((current = va_arg(args, BigInt*)))
    {
        BigInt temp = subtractBigInt(result, *current);
        freeBigInt(result);
        result = temp;
    }
    
    va_end(args);
    return result;
}

BigInt subMultipleBigInts(BigInt* first, ...) 
{
    va_list args;
    va_start(args, first);
    
    BigInt result = copyBigInt(*first);
    
    BigInt* current;
    while ((current = va_arg(args, BigInt*)))
     {  // NULL-terminated
        BigInt temp = subtractBigInt(result, *current);
        freeBigInt(result);
        result = temp;
    }
    
    va_end(args);
    return result;
}
BigInt mulMultipleBigInts(BigInt* first, ...) 
{
    va_list args;
    va_start(args, first);
    
    BigInt result = copyBigInt(*first);
    
    BigInt* current;
    while ((current = va_arg(args, BigInt*)))
     {  // NULL-terminated
        BigInt temp = multiplyBigInt(result, *current);
        freeBigInt(result);
        result = temp;
    }
    
    va_end(args);
    return result;
}

BigInt gcdMultiple(BigInt* first, ...)
{
    va_list args;
    va_start(args, first);
    
    BigInt result = copyBigInt(*first);
    
    BigInt* current;
    while ((current = va_arg(args, BigInt*)))
    {
        BigInt temp = gcdBigInt(result, *current);
        freeBigInt(result);
        result = temp;
    }
    
    va_end(args);
    return result;
}

BigInt lcmMultiple(BigInt* first, ...)
{
    va_list args;
    va_start(args, first);
    
    BigInt result = copyBigInt(*first);
    
    BigInt* current;
    while ((current = va_arg(args, BigInt*)))
    {
        BigInt temp = lcmBigInt(result, *current);
        freeBigInt(result);
        result = temp;
    }
    
    va_end(args);
    return result;
}

BigInt* maxMultiple(BigInt* first, ...)
{
    va_list args;
    va_start(args, first);
    
    BigInt* max = first;
    BigInt* current;
    
    while ((current = va_arg(args, BigInt*)))
    {
        if(isBig(*current, *max))
       {
            max = current;
        }
    }
    
    va_end(args);
    return max;
}

BigInt* minMultiple(BigInt* first, ...)
{
    va_list args;
    va_start(args, first);
    
    BigInt* min = first;
    BigInt* current;
    
    while ((current = va_arg(args, BigInt*)))
    {
        if(isSmall(*current, *min))
       {
            min = current;
        }
    }
    
    va_end(args);
    return min;
}

bool isDivisible(BigInt a, BigInt b)
{
    if (isZero(b))
    {
        fprintf(stderr, "Division by zero\n");
        exit(EXIT_FAILURE);
    }
    
    
    BigInt remainder = modBigInt(a, b);
    register int result = isZero(remainder) ? true : false;
    freeBigInt(remainder);
    return result;
}

bool isNthPerfectRoot(BigInt num, BigInt n)
{
    if (isNegative(num)) return false;
    if (isNegative(n)) 
    {
        fprintf(stderr, "Negative roots not supported\n");
        exit(EXIT_FAILURE);
    }
    
    BigInt zero = MAKE("0");
    BigInt one = MAKE("1");
    
    // 0^n = 0 and 1^n = 1 for any n
    if (isZero(num) || isEqual(num, one)) 
    {
        freeBigInt(zero);
        freeBigInt(one);
        return true;
    }

    BigInt low = MAKE("1");
    BigInt high = copyBigInt(num);
    bool found = false;

    while (compareBigInt(low, high) <= 0 && !found)
    {
        BigInt sum = addBigInt(low, high);
        BigInt mid = divideBigInt(sum, MAKE("2"));
        
        // Calculate mid^n
        BigInt power = BigPower(mid, n);
        
        int cmp = compareBigInt(power, num);
        if (cmp == 0)
        {
            found = true;
        }
        else if (cmp < 0)
        {
            BigInt temp = addBigInt(mid, one);
            freeBigInt(low);
            low = temp;
        }
        else
        {
            BigInt temp = subtractBigInt(mid, one);
            freeBigInt(high);
            high = temp;
        }
        
        freeBigInt(sum);
        freeBigInt(mid);
        freeBigInt(power);
    }

    freeBigInt(zero);
    freeBigInt(one);
    freeBigInt(low);
    freeBigInt(high);
    return found;
}

bool isPerfectCubicNumber(BigInt num)
{
    BigInt three = MAKE("3");
    bool result = isNthPerfectRoot(num, three);
    freeBigInt(three);
    return result;
}
char* bigIntToString(BigInt num)
{
    // Calculate required length: digits + sign + null terminator
    int length = num.length + (num.sign == -1 ? 1 : 0) + 1;
    char* str = (char*)calloc(length , sizeof(char));
    if (!str)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int pos = 0;
    
    // Add sign if negative
    if (num.sign == -1)
    {
        str[pos++] = '-';
    }

    // Add digits in reverse order (they're stored least significant digit first)
    for (int i = num.length - 1; i >= 0; i--)
    {
        str[pos++] = num.digits[i] + '0';
    }

    str[pos] = '\0'; // Null-terminate
    return str;
}



BigInt nextPrime(BigInt num) 
{
    // Error checking for negative input
    if (isNegative(num)) 
    {
        fprintf(stderr, "Error: Input cannot be negative\n");
        exit(EXIT_FAILURE);
    }

    // Constants we'll need
    BigInt one = MAKE("1");
    BigInt two = MAKE("2");
    BigInt four = MAKE("4");
    BigInt six = MAKE("6");

    // Handle edge cases
    if (compareBigInt(num, two) < 0) 
    {
        freeBigInt(one);
        freeBigInt(four);
        freeBigInt(six);
        return two;
    }

    // Verify input is actually prime
    if (!isPrime(num)) 
    {
        fprintf(stderr, "Warning: Input %s is not prime (finding next prime anyway)\n", 
                bigIntToString(num));
    }

    // Initialize candidate (next odd number)
    BigInt candidate;
    if (isEven(num)) 
    {
        BigInt temp = addBigInt(num, one);
        candidate = copyBigInt(temp);
        freeBigInt(temp);
    }
    else 
    {
        BigInt temp = addBigInt(num, two);
        candidate = copyBigInt(temp);
        freeBigInt(temp);
    }

    // Main search loop
    while (1) 
    {
        if (isPrime(candidate)) 
        {
            // Clean up and return found prime
            freeBigInt(one);
            freeBigInt(two);
            freeBigInt(four);
            freeBigInt(six);
            return candidate;
        }

        // Check next potential primes (6k±1 optimization)
        BigInt next = addBigInt(candidate, two);
        if (isPrime(next)) 
        {
            freeBigInt(candidate);
            freeBigInt(one);
            freeBigInt(two);
            freeBigInt(four);
            freeBigInt(six);
            return next;
        }
        freeBigInt(next);

        next = addBigInt(candidate, four);
        if (isPrime(next)) 
        {
            freeBigInt(candidate);
            freeBigInt(one);
            freeBigInt(two);
            freeBigInt(four);
            freeBigInt(six);
            return next;
        }
        freeBigInt(next);

        // Move to next candidate group
        BigInt temp = addBigInt(candidate, six);
        freeBigInt(candidate);
        candidate = temp;
    }
}
