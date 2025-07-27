#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>

// Global test counters
static int tests_run = 0;
static int tests_passed = 0;

// Function to run a test
void run_test(int (*test_func)(void), const char *test_name) {
    tests_run++;
    printf("Running %s... ", test_name);
    
    if (test_func()) {
        printf("PASSED\n");
        tests_passed++;
    } else {
        printf("FAILED\n");
    }
}

// Function to print test summary and return appropriate exit code
int test_summary(const char *suite_name) {
    printf("\n%s Results:\n", suite_name);
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);
    
    if (tests_passed == tests_run) {
        printf("All tests passed!\n");
        return 0;
    } else {
        printf("Some tests failed!\n");
        return 1;
    }
}

#endif // TEST_FRAMEWORK_H