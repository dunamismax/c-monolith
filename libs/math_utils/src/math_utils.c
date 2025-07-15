#include "math_utils.h"

int math_gcd(int a, int b) {
    // Handle negative numbers
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;
    
    // Euclidean algorithm
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int math_lcm(int a, int b) {
    if (a == 0 || b == 0) {
        return 0;
    }
    
    // LCM = (a * b) / GCD(a, b)
    int gcd = math_gcd(a, b);
    return (a / gcd) * b; // Avoid overflow by dividing first
}

int math_is_prime(int n) {
    if (n <= 1) {
        return 0; // Not prime
    }
    if (n <= 3) {
        return 1; // 2 and 3 are prime
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return 0; // Divisible by 2 or 3
    }
    
    // Check for divisors from 5 to sqrt(n)
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
    }
    
    return 1;
}

long long math_factorial(int n) {
    if (n < 0) {
        return -1; // Error: factorial undefined for negative numbers
    }
    
    long long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    
    return result;
}

long long math_fibonacci(int n) {
    if (n < 0) {
        return -1; // Error: invalid input
    }
    if (n <= 1) {
        return n;
    }
    
    long long prev = 0;
    long long curr = 1;
    
    for (int i = 2; i <= n; i++) {
        long long next = prev + curr;
        prev = curr;
        curr = next;
    }
    
    return curr;
}

long long math_power(int base, int exp) {
    if (exp < 0) {
        return 0; // Simplified: only handle non-negative exponents
    }
    if (exp == 0) {
        return 1;
    }
    
    long long result = 1;
    long long base_ll = base;
    
    // Fast exponentiation using binary representation
    while (exp > 0) {
        if (exp & 1) { // If exp is odd
            result *= base_ll;
        }
        base_ll *= base_ll;
        exp >>= 1; // Divide exp by 2
    }
    
    return result;
}

double math_sqrt(double n) {
    if (n < 0) {
        return -1; // Error: square root of negative number
    }
    if (n == 0) {
        return 0;
    }
    
    // Newton's method for finding square root
    double x = n;
    double prev;
    
    do {
        prev = x;
        x = (x + n / x) / 2.0;
    } while (math_abs(x - prev) > 1e-10);
    
    return x;
}

double math_abs(double x) {
    return (x < 0) ? -x : x;
}

int math_min(int a, int b) {
    return (a < b) ? a : b;
}

int math_max(int a, int b) {
    return (a > b) ? a : b;
}