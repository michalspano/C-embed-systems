// Includes section
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// A `Node` abstraction of a LinkedList data structure.
typedef struct n {
  unsigned long num; // We compute the sequence on n >= 0 (until some arbitrary
                     // limit, in this case MAX of `long`)
  struct n *prev;    // Pointer to the previous Node
} Node;

// Function prototypes
Node *fib(const int n);            // Compute n terms of Fibonacci sequence
_Bool is_numeric(const char* str); // Determine if a string buffer is a number
void free_linked_list(Node *head); // Linked memory occupied by LinkedList instance

/**
 * The main entry of the program. Command-line arguments are used.
 */
int main(int argc, char* argv[]) {
  // Ensure proper number of command-line arguments. Show usage if not aligned.
  if (argc != 2) {
    printf("Usage: %s <fib_n>\n", argv[0]);
    return 0;
    // Ensure that the passed argument can be converted to an integer.
  } else if (!is_numeric(argv[1])) {
    printf("Error: %s is not a number.\n", argv[1]);
    return 0;
  }
  
  // Extract the number of the Fibonacci sequence
  int n = atoi(argv[1]);
  
  // Compute the Fibonacci sequence to some n
  Node *head = fib(n);
  
  // Print the value of n in the sequence
  printf("%lu\n", head->num);

  // Free the memory occupied by the LinkedList
  free_linked_list(head);
}

// Example of the algorithm:
//
// f(0) = 0                        [1]
// f(1) = 1                        [2]
// f(2) = f(1) + f(0) = 1 + 0 = 1  [3]
// f(3) = f(2) + f(1) = 1 + 1 = 2  [4]
// f(4) = f(3) + f(2) = 2 + 1 = 3  [5]
// ...

/**
 * Compute a LinkedList of n Nodes of the Fibonacci sequence. This algorithm
 * works for all natural N_{0} (i.e., 0, 1, 2, ...). In other words:
 * ```
 * n <= 0 -> Node 0, n = 1 -> Node 0, n = 2 -> Node 1,
 * n = 3 -> Node 1, n = 4 -> Node 2, n = 5 -> Node 3,
 * ...
 * ```
 * @param n - number of Nodes to compute.
 * @returns - pointer to the head of the LinkedList
 */
Node *fib(const int n) {
  // Head of LinkedList declaration
  Node *head = (Node*)malloc(sizeof(Node));

  // f(k) = 0, k <= 0 (first Node)
  head->num  = 0;    // f(k) = 0
  head->prev = NULL; // No previous entry
 
  // Return for any n <= 0 and when n = 1.
  if (n <= 1) return head;
  
  // f(1) = 1 (second Node). Herein, the constant 1 is assigned (this is basically
  // the 'start' of the sequence).
  Node *node = (Node*)malloc(sizeof(Node)); // Allocate memory
  node->num  = 1; node->prev = head;        // Populate the fields
  head = node; // Assign new head
 
  // Exit the algorithm for n = 2 (second Node)
  if (n == 2) return head;

  // Perform the generic case, namely: f(n), n >= 2 (i.e., 3rd to n-th Nodes).
  // The generic case can now be performed, because we have the two previous
  // Nodes.
  int i = 2;        // Declare an arbitrary index
  while (i++ < n) { // Proceed to populate the remaining Nodes
    Node *node = (Node*)malloc(sizeof(Node)); // Allocate memory
    node->prev = head;                        // Assign previous node as the current head

    // f(n)    = f(n-1)    + f(n-2)           [recurrent sequence]
    node->num  = head->num
               + head->prev->num;

    head = node; // Assign new head
  }

  // Return the current head
  return head;
}
/**
 * A subroutine to clear occupied memory (by a LinkedList) that has been
 * dynamically allocated. The LinkedList is populated with `Node` entries.
 *
 * @head - the first node of the LinkedList
 */
void free_linked_list(Node *head) {
  Node *tmp;                     // Temporary Node declaration
  while ((tmp = head) != NULL) { // Proceed until the Node is NULL
    head = head->prev;           // Update the head with the previous node
    free(tmp);                   // Free the memory occupied by the current node
  }
}

/**
 * A function to determine whether a string buffer is a valid integer value.
 *
 * @param str - the string buffer to be evaluated
 * @returns   - a boolean value indicating whether the string buffer is an integer
 *              or not.
 */
_Bool is_numeric(const char* str) {
  // Be mindful of negative values; an integer either starts with '-' or a digit.
  if (str[0] != '-' && !isdigit(str[0])) return 0;

  // Proceed with the rest of the string. All positions must be digits
  for (int i = 1; i < strlen(str); i++) {
    if (!isdigit(str[i])) return 0; // Not a digit: return false
  }

  // This is an integer, return true.
  return 1;
}
