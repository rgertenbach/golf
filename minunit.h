#include <stdio.h>

#define MU_NO_CLEANUP() do { \
  (void) 0; \
} while (0)

// Runs a test and prints the message if it fails
//
// @param message The name of the test / message to send when the test fails.
// @param test A c expression to run, falsy values mean a failure.
// @param cleanup Optional cleanup freeing memory.
#define mu_test(message, test, cleanup) do { \
  if (!(test)) { \
    cleanup(); \
    return message; \
  } \
} while (0)

// Calls function which runs a test checking if it was successful.
//
// @param test The test function to run. Must take void.
#define mu_run_test(test) do { \
  printf(#test ": "); \
  char *failed = test(); \
  tests_run++; \
  if (failed != NULL) { \
    tests_failed++; \
    printf("FAILED: %s\n", failed); \
  } else printf("passed\n"); \
} while (0)

// Global keeping track of the number of tests run.
int tests_run = 0;
int tests_failed = 0;

// Run tests.
//
// @param suite A void function taking void running all tests.
#define mu_main(suite) do { \
  suite(); \
  printf("Ran %d tests\n", tests_run); \
  if (!tests_run) printf("No tests run\n"); \
  if (tests_failed) { \
    printf("%d tests failed\n", tests_failed); \
  } \
  else printf("All tests pass! :)\n"); \
} while (0)

// The absolute value of an expression.
#define mu_abs(x) (((x) < 0) ? -(x) : (x))

// Checks for equality between a and b within a certain error epsilon.
// This is to account for floating point errors.
#define mu_approx_equal(a, b, epsilon) (mu_abs((a) - (b)) < epsilon)

// Checks for exact equality.
#define mu_test_equal(message, expected, actual, cleanup) do { \
  if ((expected) != (actual)) { \
    char *mu_result = #actual " should be " #expected " but isn't"; \
    cleanup(); \
    return mu_result; \
  } \
} while (0)
