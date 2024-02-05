// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 2
// Exercise 2
// Submission code: <XXXYYY>

// Includes section
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Macros section
#define INIT_SIZE 5 // The number of nodes in the initial state.
#define MAX_VAL 100 // The highest possible value in the initial random
                    // computation of a linked list (MAX in the template).

// Type definitions

// A doubly-linked list abstraction.
typedef struct q {
  int number;     // An int value that the node holds.
  struct q *next; // A pointer to the next node.
  struct q *prev; // A pointer to the previous node.
} REGTYPE;

// Function prototypes (API)
int random_int(void);                        // Generate random int in a range.
REGTYPE* random_list(void);                  // Generate a random linked list given properties.
void freeLinkedList(REGTYPE *head);          // Free memory occupied by a linked list.
void printLinkedList(REGTYPE *head);         // Print the values of a linked list.
REGTYPE* add_first(REGTYPE* temp, int data); // Add a new node to the topmost position of a linked list.

/**
 * Main program section. The program does not use any command-line arguments.
 * The program uses additional subroutines, so it uses a different structure
 * than the provided template.
 */
int main(void) {
  srand(time(0)); // Reset the seed to achieve pseudo-random values given each
                  // execution of the program.

  // Initial computation of a linked list with values ranging from
  // [0, MAX_VAL].
  REGTYPE* head = random_list();

  // Manually add some integers to the computed linked list (test).
  // Replace by 1 to allow.
#if 0
  head = add_first(head, 1); // [4]
  head = add_first(head, 8); // [3]
  head = add_first(head, 6); // [2]
  head = add_first(head, 0); // [1]
  head = add_first(head, 1); // [0]
  // Total new length = 10
#endif

  printLinkedList(head); // Print the entries of the given linked list.
  freeLinkedList(head);  // Free the memory that the linked list occupies.

  // Exit with 0 to indicate a successful completion.
  return 0;
}

/**
 * A function to generate a linked list of size `INIT_SIZE` with random values
 * ranging from [0, MAX_VAL].
 *
 * @see random_int()
 * @returns - a pointer to the head of the newly created linked list.
 */
REGTYPE* random_list() {
  // Declare pointers of type Node that will be used in the function.
  REGTYPE *top, *old, *item;

  // Reserve memory for the topmost node from the list. This is required
  // outside the loop, since the `old` pointer will be used to store the
  // previous node from the list.
  top = malloc(sizeof(REGTYPE));

  old = top;                  // Store the pointer to `old` (will be
                              // significant in the loop's body).
  old->prev = NULL;           // The previous entry of the head is null.
  old->number = random_int(); // Add a randomly computed int to the node.

  // Iterate `INIT_SIZE` times in a for-loop.
  for (int i = 0; i < INIT_SIZE - 1; i++) {
    item = malloc(sizeof(REGTYPE)); // Reserve memory for a new Node assigned to item.
    item->number = random_int();    // Add a randomly computed int to the item.
  
    old->next = item; // Make the previous node have it's `next` pointer to the
                      // current item.
    item->prev = old; // Do the opposite; the current item has the `prev`
                      // pointer as the previous item from the list.
    old = item;       // Done. Store the current item in `old` for the new
                      // iteration of the loop.
  }

  // Ultimately, return the topmost (head) pointer.
  return top;
}
/**
 * A function to add a new node entry to the first position of the linked list.
 * @param temp - a temporary 'head' from the current list.
 * @param data - an integer value added to the newly inserted node.
 *
 * @returns - the new head from the linked list, that is, a pointer to the
 * newly inserted node.
 */
REGTYPE* add_first(REGTYPE* temp, int data) {
  REGTYPE *n = malloc(sizeof(REGTYPE)); // Dynamically allocate one Node on the heap,
                                  // retain its pointer.
  n->number = data;               // Set the `number` parameter to provided data.
  n->next   = temp;               // Assign `temp`'s pointer as the `next` of
                                  // the new node.
  temp->prev = n;                 // Assign `temp`'s previous node as the newly
                                  // created node.

 return n;                        // Return a pointer to the new node (i.e., the new 'head')
}

/**
 * A subroutine to print the entries from the head to the tail of a given
 * linked list.
 *
 * @param head - a pointer to the first node of the linked list (i.e., the 'head').
 */
void printLinkedList(REGTYPE* head) {
  int nr = 1; // Display the values with an index

  // Continue while the head is not null.
  while (head != NULL) {
    // Print the value from the node, increment i.
    printf("Post nr %d: %d\n", nr++, head->number);
    head = head->next; // Update the current 'head' pointer by the consecutive
                       // entry.
  }
}

/**
 * A subroutine to free the whole sequence of a given linked List.
 * @param head - a pointer to the first node of the linked list (i.e., the 'head').
 */
void freeLinkedList(REGTYPE* head) {
  REGTYPE *tmp = head; // Create a temporary copy of the head.

  // Proceed to iterate until the end of the list is reached.
  while ((tmp = head) != NULL) {
    head = tmp->next; // Assign the head with the consecutive pointer.
    free(tmp);        // Free the memory of the current node.
  }
}

/**
 * A helper function to generate a random integer in the desired interval.
 * The `MAX_VAL` macro is used in this case, and the `rand()` computes a random
 * int.
 *
 * @returns - a random integer in the range [0, MAX_VAL].
 */
int random_int() {
  return rand() % (MAX_VAL + 1);
}
