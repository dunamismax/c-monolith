#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

// Include the math utils library
#include "../../libs/math_utils/include/math_utils.h"

int tests_run = 0;
int tests_passed = 0;

void run_test(int (*test_function)(void), const char* test_name) {
    tests_run++;
    printf("Running test: %s ... ", test_name);
    if (test_function()) {
        printf("PASS\n");
        tests_passed++;
    } else {
        printf("FAIL\n");
    }
}

// Test GCD function
int test_gcd(void);

// Test LCM function
int test_lcm(void);

// Test prime checking
int test_is_prime(void);

// Test factorial function
int test_factorial(void);

// Test Fibonacci function
int test_fibonacci(void);

// Test power function
int test_power(void);

// Test square root function
int test_sqrt(void);

// Test absolute value function
int test_abs(void);

// Test min and max functions
int test_min_max(void);

int main(void)
{
    printf("Math Utils Library Test Suite\n");
    printf("============================= \n\n");

    run_test(test_gcd, "test_gcd");
    run_test(test_lcm, "test_lcm");
    run_test(test_is_prime, "test_is_prime");
    run_test(test_factorial, "test_factorial");
    run_test(test_fibonacci, "test_fibonacci");
    run_test(test_power, "test_power");
    run_test(test_sqrt, "test_sqrt");
    run_test(test_abs, "test_abs");
    run_test(test_min_max, "test_min_max");

    printf("\nTest Results:\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);

    if (tests_passed == tests_run)
    {
        printf("\nAll tests passed! \u2713\n");
        return 0;
    }
    else
    {
        printf("\nSome tests failed! \u2718\n");
        return 1;
    }
}
