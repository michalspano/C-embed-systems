// (C) Erik Lindstrand, Konstantinos Rokanas, Michal Spano, group: 5 (2024)
// Work package 4
// Exercise 4
// Submission code: <XXXYYY>

#include <stdio.h>                                      // header for standard library containing functions for input and output

// function declarations
int searchNumber(int number, int tab[], int size);       // determines if int number exists in an array and at what index
void sort(int number, int tab[], int length);           // sorts elements of array in numerical order
void swapElements(int* a, int* b);                      // helper that swaps array elements pointed at by two pointers
void printArray(char* prefix, int *tab, int length);    // helper that prints contents of array

// main program logic

/**
* Main function executed by the program.
*
* @function main
* @param int argc - number of user inputted arguments in command line
* @param char *argv[] - array of pointers to Strings containing each argument (index 0 is always program name)
* @return int - exit code, 0 == successful program execution and 1 == program termination with error
**/
int main (void) {

    // variable declarations
    int n = 13;                                           // define an integer
    int test[] = {1, 2, 34, 5, 67, 3, 23, 12, 13, 10};    // initialize an array of integers
    int size = sizeof(test);                              // determine size of array (in bytes)
    int length = size / sizeof(int);                      // determine length of array by dividing its size by the size of one elemnet

    // call helper function to print the unsorted array
    printArray("Unsorted array:", test, length);

    // call function to find if integer n exists in test[] array
    // store index of first occurrence of n in variable
    int firstIndex = searchNumber(n, test, size);

    // print resulting first occurence (-1 signifies number not present in String)    
    printf("Index of n (%d): %d\n", n, firstIndex);

    // call function to sort test[] array in numerical order  
    sort(n, test, length);

    // call helper function to print the sorted array
    printArray("Sorted array:", test, length);

    // return exit code signifying successful program execution
    return 0;
}

/**   
* determine if number exists in an array and at what index
* 
* @function searchNumber
* @param int number - number being searched for in array
* @param int tab[] - unsorted array of numbers being searched through
* @param int size - size of array (in bytes)
* @return int - index of first occurrence of number in array (-1 if number is not present in array)
**/
int searchNumber(int number, int tab[], int size) {

    // determine length of array by dividing its size by the size of one element
    int length = size / sizeof(int);

    // loop through the length of array
    for(int i = 0; i < length; i++) {

        // if any of the elements match the number
        if(tab[i] == number) {

            // return the index of this occurrence
            return i;
        }
    }

    // if entire array is traversed without finding a matching number, return -1
    return -1;
}

/**   
* sorts elements of array in numerical order
* 
* @function sort
* @param int number - I'M NOT ACTUALLY SURE WHAT THIS IS MEANT TO BE!!!
* @param int* tab - pointer to the first element of array to be sorted
* @param int length - number of elements in array  - TECHNICALLNY NOT PART OF ORIGINAL SIGNATURE!!!
**/
void sort(int number, int tab[], int length) {

    // declare variable to store the index of the smallest element in array
    int minIndex;

    // loop through length of array
    // elements at index i will be compared against elements at index j
    for(int i = 0; i < length - 1; i++) {

        // each loop, reset the smallest index to the new value of i
        minIndex = i;

        // loop through length of array
        // elements at index j will be compared against the current leftmost element (index i)
        for(int j = i; j < length; j++) {

            // if the current element at index j is numerically smaller than the element at minIndex
            if(tab[j] < tab[minIndex]) {

                // set the new minIndex to index j
                minIndex = j;
            }
        }

        // after minimum value is determined for this loop, swap it with current leftmost value (at index i)
        swapElements(&tab[i], &tab[minIndex]);
    }
}

/**   
* Swaps the values of characters at two pointers.
* 
* @function swapElements
* @param int* a - pointer to first integer element
* @param int* b - pointer to second integer element
**/
void swapElements(int* a, int* b) { 

    // declare temporary storage of a character  
    char tempChar; 

    // set the value of tempChar to the value pointed at by pointer a
    tempChar = *a; 

    // change the value pointed at by pointer a to value pointed at by pointer b
    *a = *b; 

    // change the value pointed at by pointer b to value stored in tempChar
    *b = tempChar; 
} 

/**   
* print contents of array, formatted and with a String prefix
* 
* @function printArray
* @param char* prefix - String to be printed before the array
* @param int* tab - pointer to the first element of array to be printed
* @param int length - length of array to be printed
**/
void printArray(char* prefix, int *tab, int length) {

    // print String prefix and open curly bracket
    printf("%s { ", prefix);

    // loop through test[] array
    for(int i = 0; i < length; i++) {

        // if current element is final element in array
        if(i == length - 1) {

            // print array element at current index without a comma
            printf("%d ", tab[i]);

        // if current element is at any position other than final
        } else {

            // print array element at current index
            printf("%d, ", tab[i]);
        }
    }

    // after printing all elements, print a closing curly bracket
    printf("}\n");
}