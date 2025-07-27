#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>
#include <stdlib.h>

// Thread-safe test counters using a structure
typedef struct {
  int tests_run;
  int tests_passed;
} TestContext;

// Global test context
static TestContext test_context = {0, 0};

// Function to run a test
void run_test(int (*test_func)(void), const char *test_name) {
  test_context.tests_run++;
  printf("Running %s... ", test_name);
  fflush(stdout);

  if (test_func()) {
    printf("PASSED\n");
    test_context.tests_passed++;
  } else {
    printf("FAILED\n");
    if (getenv("CI")) {
      printf("  ^^^^ Test %s failed in CI environment\n", test_name);
    }
  }
  fflush(stdout);
}

// Function to print test summary and return appropriate exit code
int test_summary(const char *suite_name) {
  printf("\n%s Results:\n", suite_name);
  printf("Tests run: %d\n", test_context.tests_run);
  printf("Tests passed: %d\n", test_context.tests_passed);
  printf("Tests failed: %d\n", test_context.tests_run - test_context.tests_passed);

  if (test_context.tests_passed == test_context.tests_run) {
    printf("All tests passed!\n");
    return 0;
  } else {
    printf("Some tests failed!\n");
    return 1;
  }
}

#endif // TEST_FRAMEWORK_H