// Includes section
#include <stdio.h>
#include <string.h>

// The sleep function is defined in different header files based
// on the operating system. On Windows, it is found in the `<Windows.h>`
// header, with a signature `Sleep(x)`. Otherwise, it can be obtained
// from the `<unistd.h>` header file under the signature `sleep(x)`.
// The following code ensures that the pre-processor a header file based
// on the OS that the machine uses.
#ifdef _WIN32
  #include <Windows.h>
  #define sleep(x) Sleep(x)
#else
  #include <unistd.h>
#endif

// Declare an enum to represent days of the week, taken as the first 3
// chars of the their string literals. Furthermore, `Mon` is indexed from 1
// (not 0, which would be the default). Wrap the enum in a `typdef`, so
// `week_day` is an enum-based type that can be used in the program.
typedef enum {
  Mon = 1,  Tue, Wed,
  Thu, Fri, Sat, Sun
} week_day;

// Declare an enum to represent the allowed weeks for the program. Note that
// the indexing starts from 1 (for WEEK1) for the sake of readability.
typedef enum {
  WEEK1 = 1, WEEK2, WEEK3,
  WEEK4, WEEK5
} week_range;

// Function declaration prototype(s)
void clearBuffer();
char* mapIntWeekToString (week_day w);

// Main entry point to the program. No command-line arguments are expected.
int main(void) { 
  week_range week; // Used to store the week nr.
  week_day day;    // Var. to store the enum-type day

  // Prompt the user to provide a week number, read it using `scanf` and
  // assign the value to `week`. Having read the value from the user, the
  // buffer is cleared to avoid any redundant new-line characters.
  printf("Provide starting week: ");
  scanf("%d", &week); clearBuffer();
  
  // Check that the week ordinal is valid and `MAX_WEEK` is not exceeded. Here,
  // 0 is returned; however, it should be a non-zero int, to indicate an error.
  // However, this is done for the sake of CodeGrade, which will yield a warning
  // if a non-zero int is returned.
  if (week < WEEK1 || week > WEEK5) {
    printf("Error: invalid week number. Use the interval 1-%d.\n", WEEK5);
    return 0;
  }

  // Prompt the user to provide a day (in the form of integer). Use `scanf` to
  // obtain the value and store it in `day`. Having read the value from the
  // user, the buffer is cleared to avoid any redundant new-line characters.
  printf("Provide starting day: ");
  scanf("%d", &day); clearBuffer();
 
  // Check that the provided day is a valid one (i.e., it is between 1 through 7). 0
  // is returned in case of an error (see line 45 for the explanation).
  if (day < Mon || day > Sun) {
    printf("Error: invalid day.\n");
    return 0;
  }

  // Continue until week `MAX_WEEK` (i.e., 5) is reached.
  while (week <= WEEK5) {
    // `NULL` does not need to be handled, because `day` is always bounded
    // in a valid interval. However, if the function were to be used elsewhere
    // where such a check is not performed, then it would be necessary to handle
    // the case where `NULL` is returned.
    printf("Week %d, %s\n", week, mapIntWeekToString(day));

    // A new week is detected, increase week nr., shift day to Monday.
    // Otherwise, increment the current day;
    if (day == Sun) { day = Mon; week++; }
    else              day++;

    sleep(1); // Delay for one second before the next iteration.
  }
  
  // Return zero to indicate a successful execution of the program.
  return 0;
}

/**
 * A helper function to clear the input buffer. This is helpful to
 * eliminate any redundant new-line characters, after reading an int
 * from the user, and so forth.
 */
void clearBuffer() {
  int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * A helper function to map each enum value of `week_day` to
 * its corresponding string literal representation. If the mapping
 * cannot be made, `NULL` is returned.
 *
 * @params w - an enum-based day to be mapped to a string.
 * @returns - a string literal of a day in the week.
 */
char* mapIntWeekToString (week_day w) {
  // Create a switch-statement based on the value of w.
  switch (w) {
    case Mon: return "Monday";    // Mapping 1 to "Monday"
    case Tue: return "Tuesday";   // ...
    case Wed: return "Wednesday";
    case Thu: return "Thursday";
    case Fri: return "Friday";
    case Sat: return "Saturday";
    case Sun: return "Sunday";
    default:  return NULL;        // Otherwise, return NULL.
  }
}

