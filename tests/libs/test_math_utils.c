#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Include the math utils library
#include "../../libs/math_utils/include/math_utils.h"

/**
 * Simple test framework for the math utils library
 */

int tests_run = 0;
int tests_passed = 0;

#define TEST(name)                             \
    do                                         \
    {                                          \
        printf("Running test: %s ... ", name); \
        tests_run++;                           \
        if (test_##name())                     \
        {                                      \
            printf("PASS\n");                  \
            tests_passed++;                    \
        }                                      \
        else                                   \
        {                                      \
            printf("FAIL\n");                  \
        }                                      \
    } while (0)

// Test GCD function
int test_gcd()
{
    if (math_gcd(12, 18) != 6)
        return 0;
    if (math_gcd(48, 18) != 6)
        return 0;
    if (math_gcd(7, 13) != 1)
        return 0; // Prime numbers
    if (math_gcd(0, 5) != 5)
        return 0;
    if (math_gcd(5, 0) != 5)
        return 0;
    if (math_gcd(-12, 18) != 6)
        return 0; // Negative numbers
    if (math_gcd(12, -18) != 6)
        return 0;
    return 1;
}

// Test LCM function
int test_lcm()
{
    if (math_lcm(4, 6) != 12)
        return 0;
    if (math_lcm(3, 5) != 15)
        return 0;
    if (math_lcm(12, 18) != 36)
        return 0;
    if (math_lcm(0, 5) != 0)
        return 0;
    if (math_lcm(5, 0) != 0)
        return 0;
    return 1;
}

// Test prime checking
int test_is_prime()
{
    // Test small primes
    if (!math_is_prime(2))
        return 0;
    if (!math_is_prime(3))
        return 0;
    if (!math_is_prime(5))
        return 0;
    if (!math_is_prime(7))
        return 0;
    if (!math_is_prime(11))
        return 0;
    if (!math_is_prime(13))
        return 0;

    // Test non-primes
    if (math_is_prime(1))
        return 0; // 1 is not prime
    if (math_is_prime(4))
        return 0;
    if (math_is_prime(6))
        return 0;
    if (math_is_prime(8))
        return 0;
    if (math_is_prime(9))
        return 0;
    if (math_is_prime(10))
        return 0;

    // Test larger numbers
    if (!math_is_prime(17))
        return 0;
    if (!math_is_prime(19))
        return 0;
    if (math_is_prime(21))
        return 0; // 3 * 7
    if (math_is_prime(25))
        return 0; // 5 * 5

    // Test edge cases
    if (math_is_prime(0))
        return 0;
    if (math_is_prime(-5))
        return 0;

    return 1;
}

// Test factorial function
int test_factorial()
{
    if (math_factorial(0) != 1)
        return 0; // 0! = 1
    if (math_factorial(1) != 1)
        return 0; // 1! = 1
    if (math_factorial(3) != 6)
        return 0; // 3! = 6
    if (math_factorial(4) != 24)
        return 0; // 4! = 24
    if (math_factorial(5) != 120)
        return 0; // 5! = 120

    // Test negative input
    if (math_factorial(-1) != -1)
        return 0; // Should return error

    return 1;
}

// Test Fibonacci function
int test_fibonacci()
{
    if (math_fibonacci(0) != 0)
        return 0;
    if (math_fibonacci(1) != 1)
        return 0;
    if (math_fibonacci(2) != 1)
        return 0;
    if (math_fibonacci(3) != 2)
        return 0;
    if (math_fibonacci(4) != 3)
        return 0;
    if (math_fibonacci(5) != 5)
        return 0;
    if (math_fibonacci(6) != 8)
        return 0;
    if (math_fibonacci(7) != 13)
        return 0;
    if (math_fibonacci(10) != 55)
        return 0;

    // Test negative input
    if (math_fibonacci(-1) != -1)
        return 0; // Should return error

    return 1;
}

// Test power function
int test_power()
{
    if (math_power(2, 0) != 1)
        return 0; // Any number to power 0 is 1
    if (math_power(2, 1) != 2)
        return 0;
    if (math_power(2, 3) != 8)
        return 0;
    if (math_power(3, 2) != 9)
        return 0;
    if (math_power(5, 3) != 125)
        return 0;
    if (math_power(10, 2) != 100)
        return 0;

    // Test edge cases
    if (math_power(0, 1) != 0)
        return 0;
    if (math_power(1, 100) != 1)
        return 0;

    // Test negative exponent (should return 0 for simplicity)
    if (math_power(2, -1) != 0)
        return 0;

    return 1;
}

// Test square root function
int test_sqrt()
{
    // Test perfect squares
    double result;

    result = math_sqrt(0);
    if (result != 0)
        return 0;

    result = math_sqrt(1);
    if (result < 0.999 || result > 1.001)
        return 0;

    result = math_sqrt(4);
    if (result < 1.999 || result > 2.001)
        return 0;

    result = math_sqrt(9);
    if (result < 2.999 || result > 3.001)
        return 0;

    result = math_sqrt(16);
    if (result < 3.999 || result > 4.001)
        return 0;

    // Test non-perfect square
    result = math_sqrt(2);
    if (result < 1.414 || result > 1.415)
        return 0;

    // Test negative input (should return -1)
    result = math_sqrt(-1);
    if (result != -1)
        return 0;

    return 1;
}

// Test absolute value function
int test_abs()
{
    if (math_abs(5.0) != 5.0)
        return 0;
    if (math_abs(-5.0) != 5.0)
        return 0;
    if (math_abs(0.0) != 0.0)
        return 0;
    if (math_abs(3.14) != 3.14)
        return 0;
    if (math_abs(-3.14) != 3.14)
        return 0;
    return 1;
}

// Test min and max functions
int test_min_max()
{
    if (math_min(5, 3) != 3)
        return 0;
    if (math_min(3, 5) != 3)
        return 0;
    if (math_min(-5, -3) != -5)
        return 0;
    if (math_min(0, 5) != 0)
        return 0;
    if (math_min(5, 5) != 5)
        return 0;

    if (math_max(5, 3) != 5)
        return 0;
    if (math_max(3, 5) != 5)
        return 0;
    if (math_max(-5, -3) != -3)
        return 0;
    if (math_max(0, 5) != 5)
        return 0;
    if (math_max(5, 5) != 5)
        return 0;

    return 1;
}

int main()
{
    printf("Math Utils Library Test Suite\n");
    printf("=============================\n\n");

    TEST(gcd);
    TEST(lcm);
    TEST(is_prime);
    TEST(factorial);
    TEST(fibonacci);
    TEST(power);
    TEST(sqrt);
    TEST(abs);
    TEST(min_max);

    printf("\nTest Results:\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);

    if (tests_passed == tests_run)
    {
        printf("\nAll tests passed! ✓\n");
        return 0;
    }
    else
    {
        printf("\nSome tests failed! ✗\n");
        return 1;
    }
}