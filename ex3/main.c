/**
* This program presents a menu from which the user can select one of four subroutines.
* The first creates a new binary file and fills it with dummy data (replacing previous file if it exists).
* The second allows the user to append a new person entry to the file (using user input to populate the attributes).
* The third allows the user to search for specific persons in the file by first or family name.
* The fourth reads all entries in the file and prints them.
* The fifth option exits the program.
* 
* Purpose: to demonstrate basic knowledge of writing and reading binary files in C.
* DIT633
* 
* Authors: Group 5, 2024
**/

#include <stdlib.h>     // header for standard library necessary exit() function
#include <stdio.h>      // header for standard library containing functions for input and output
#include <string.h>     // header for standard library containing functions for manipulation of Strings
#include <stdbool.h>    // header for standard library necessary for boolean type support

// typedefs

// this struct represents a type PERSON that will be stored in the binary file
typedef struct {
    char firstname[20];     // string representing first name
    char famname[20];       // string representing family name
    char pers_number[13];   // string representing personal number / yyyymmddnnnc
} PERSON;

// function declarations
void clearBuffer(void);                             // clears input buffer to ensure no leftover \n
bool isEmpty(FILE *fileptr);                        // checks if a file is empty
void write_new_file(PERSON *inrecord);              // creates file and writes the first record
PERSON input_record(void);                          // reads a person's record
void append_file(PERSON *inrecord);                 // appends a new person to the file
bool search_by_name(char *name);                    // prints out the person if in list
void printfile(void);                               // prints out all persons in the file

int main(void) {

    // variable declarations
    PERSON ppost;                                   // variable to store temporary person struct
    char taskChoice;                                // variable storing user input denoting which task is to be run
    char searchChoice;                              // variable storing user input denoting whether to keep searching for person
    char searchInput[20];                           // variable storing user input denoting name String to search for
    bool entryFound;                                // flag denoting that person with matching name exists in file (for task 3) 

    // print the task choices
    printf("-----------------------------\n");
    printf("Welcome to Exercise 3! \n");
    printf("1. Create a new and delete the old file.\n");
    printf("2. Add a new person to the file.\n");
    printf("3. Search for a person in the file.\n");
    printf("4. Print out all in the file.\n");
    printf("5. Exit the program.\n");

    // single loop represents one task selection
    // loop continues until 'Exit the program' is chosen (input '5')
    do {
        // print divider line to make output more readable and prompt user to select task
        printf("-----------------------------\n");
        printf("Please choose an option between 1 and 5: ");

        // record user input denoting choice of task to run
        taskChoice = getchar();

        // switch case depending on user input
        switch(taskChoice) {

            // case for "Create a new and delete the old file"
            case '1':

                // dummy data stored temporarily in variable, to be recorded as first entry in the file
                ppost = (PERSON){"Duke", "Nukem", "199409021234"};

                // call function to create a file and write the first record
                // takes memory address of declared person struct as parameter
                write_new_file(&ppost);

                // exit the switch block after case is executed
                break;

            // case for "Add a new person to the file."
            case '2':

                // call function to create new person struct using user input
                // function returns a person struct and stores it in ppost variable
                ppost = input_record();

                // call function to append user-created person struct in the file
                append_file(&ppost);

                // exit the switch block after case is executed
                break;

            // case for "Search for a person in the file."
            case '3':

                // always execute first iteration of loop 
                do {

                    // clear input buffer to ensure no leftover \n
                    clearBuffer();

                    // print message to prompt user input
                    printf("Please type name to search: ");

                    // record user input to be used as search term 
                    // input will only record as many characters as can fit in firstname/famname attributes of PERSON
                    scanf("%19s", searchInput);

                    // call function to read file and search for and print records where firstname or famname attributes match search term
                    // function returns a boolean denoting whether any matches were found, and this is stored in flag variable
                    entryFound = search_by_name(searchInput);

                    // if the search function yielded no matching results
                    if(!entryFound) {

                        // clear input buffer to ensure no leftover \n
                        clearBuffer();

                        // print message to prompt user input
                        printf("Try again? (Y/N): ");

                        // capture user input denoting whether user wishes to search again or return to program menu ('Y' or 'N' respectively)
                        searchChoice = getchar();
                        
                        // as long as user does not submit valid input (lowercase or uppercase 'Y' or 'N' character), stay within below loop
                        while(searchChoice != 'N' && searchChoice != 'n' &&
                              searchChoice != 'Y' && searchChoice != 'y') {

                            // print message to prompt user to submit a valid input
                            printf("Invalid input. Please choose Y or N: ");

                            // clear input buffer to ensure no leftover \n
                            clearBuffer();

                            // recapture user input denoting whether user wishes to search again or return to program menu ('Y' or 'N' respectively)
                            searchChoice = getchar();
                        }
                    }

                // repeat loop if search yielded no matches and user has chosen to search again ('Y' input)
                } while (searchChoice == 'Y' || searchChoice == 'y' && entryFound != true);
        
                // exit the switch block after case is executed
                break;

            // case for "Print out all in the file."
            case '4':
                
                // function that reads through every record in the file and prints them 
                printfile();

                // exit the switch block after case is executed
                break;

            // case for "Exit the program."
            case '5':

                // print affirmative message
                printf("Thank you for using our program.\n");
                
                // exit the switch block after case is executed
                break;

            // case for input that is anything other than chars 1-5
            default:

                // print negative message
                printf("Invalid input.\n");
                
                // exit the switch block after case is executed
                break;
        }

    // check if user input contains only a single \n; if so, input buffer should not be cleared
    // this is relevant when the user inputs ENTER with no other characters
    if(taskChoice != '\n') {

        // clears the buffer to ensure no \n characters interfere with subsequent inputs
        clearBuffer();
    }
    
    // loop the program, allowing user to keep selecting tasks until they select to close the program
    } while(taskChoice != '5');

    // return exit code signifying program success
    return 0;
}

// clears the buffer to ensure no \n characters interfere with subsequent inputs
// adapted from https://stackoverflow.com/questions/7898215/how-can-i-clear-an-input-buffer-in-c
void clearBuffer() {

    // consume consecutive characters from buffer in a loop until encountering \n or end of file
    while ((getchar()) != '\n' && getchar() != EOF) { }
}

// checks if a file is empty
// takes file pointer as a parameter and returns a boolean
bool isEmpty(FILE *fileptr) {

    // start by setting file pointer to the end of the file using SEEK_END reference point
    // 0 denotes no offset from the reference point
    fseek(fileptr, 0, SEEK_END);

    // check the current position of the file pointer
    // since pointer is set to the end of the file, its position will denote the size of the file
    // if file size is 0, the file is empty
    if (ftell(fileptr) == 0) {

        // print relevant message
        printf("The file is empty. \n");

        // return true denoting file is empty
        return true;
    }

    // move the file pointer back to the beginning of the file using SEEK_SET reference point
    // 0 denotes no offset from the reference point
    fseek(fileptr, 0, SEEK_SET);

    // return false denoting file is not empty
    return false;
}

// create file and write the first record
// takes pointer to PERSON variable initialized with dummy data
void write_new_file(PERSON *inrecord) {   

    // declare a file pointer
    FILE *fileptr;
    
    // attempt to open a file named persons.bin in binary write mode ("wb")
    if((fileptr=fopen("persons.bin","wb")) == NULL) {

        // if the file opening fails, print an error message 
        printf("Cannot create the file\n");

        // exit the program with status code 1 denoting unsuccessful program execution
        exit(1);
    }

    // write the provided person record to the binary file
    // specify the entry should be equal to the size of the PERSON struct
    // specify that a single entry of that size should be written
    fwrite(inrecord, sizeof(PERSON), 1, fileptr);

    // close the file 
    fclose(fileptr);

    // print affirmative message
    printf("File successfully created.\n");

}      

// create new person struct using user input
// function returns a person struct and stores it in ppost variable
PERSON input_record(void) {

    // initialize temporary PERSON variable to store user input
    PERSON tempRecord = (PERSON){};

    // print message to prompt user input for first name
    printf("Please enter first name: ");

    // record user input for first name (only as many characters as can fit in firstname attribute of PERSON)
    scanf("%19s", tempRecord.firstname);    

    // clear input buffer to ensure no leftover \n
    clearBuffer();

    // print message to prompt user input for family name
    printf("Please enter family name: ");

    // record user input for family name (only as many characters as can fit in famname attribute of PERSON)
    scanf("%19s", tempRecord.famname); 

    // clear input buffer to ensure no leftover \n
    clearBuffer();

    // print message to prompt user input for personal number
    printf("Please enter personal number: ");

    // record user input for personal number (only as many characters as can fit in pers_number attribute of PERSON)
    scanf("%12s", tempRecord.pers_number);  

    // return the new PERSON data containing user input
    return tempRecord;
};

// function to append user-created person record in the file
void append_file(PERSON *inrecord) {

    // declare a file pointer
    FILE *fileptr;
    
    // attempt to open a file named persons.bin in binary append mode ("ab")
    if((fileptr=fopen("persons.bin","ab")) == NULL) {

        // if the file opening fails, print an error message 
        printf("Cannot create the file\n");

        // exit the program with status code 1 denoting unsuccessful program execution
        exit(1);
    }


    // write the provided person record to the binary file
    // specify the entry should be equal to the size of the PERSON struct
    // specify that a single entry of that size should be written
    fwrite(inrecord, sizeof(PERSON), 1, fileptr);

    // close the file
    fclose(fileptr);

    // print affirmative message
    printf("Person successfully recorded in file.\n");
}

// read file and search for and print records where firstname or famname attributes match search term
// function returns a boolean denoting whether any matches were found
bool search_by_name(char *name) {

    // variable declarations
    FILE *fileptr;              // declare a file pointer
    PERSON tempRecord;          // declare PERSON variable to store temporary person record
    bool entryFound = false;    // initialize flag denoting whether matching person record has been found with value 'false'

    // attempt to open a file named persons.bin in binary read mode ("rb")
    if((fileptr=fopen("persons.bin","rb")) == NULL) {

        // if the file opening fails, print an error message 
        printf("Cannot create the file\n");

        // exit the program with status code 1 denoting unsuccessful program execution
        exit(1);
    }

    // check if file is empty; if not, proceed with subroutine
    if (isEmpty(fileptr) == false) {

        // iterate through and read each record until the end of the file
        // if fread successfully reads one PERSON element, it returns 1
        // if it fails to read an element (e.g. by reaching end of file), it returns 0
        while (fread(&tempRecord, sizeof(PERSON), 1, fileptr) == 1) {

            // check if either firstname or famname of current person entry matches the provided search term
            if (strcmp(tempRecord.firstname, name) == 0 || strcmp(tempRecord.famname, name) == 0) {

                // if a matching entry is found, set the entryFound flag to true
                entryFound = true;

                // print divider line and current matching person data 
                printf("-----------------------------\n");
                printf("First Name: %s\n", tempRecord.firstname);
                printf("Last Name: %s\n", tempRecord.famname);
                printf("Personal Number: %s\n", tempRecord.pers_number);
            }
        }

        // if no matching person entries were found in file
        if(entryFound == false) {

            // print negative message
            printf("No matching entries found.\n");

            // return boolean denoting failure to find matching person
            return false;
        }

        // if matching person entry was found (entryFound flag was set to true), return true
        return true;
    }

    // close the file
    fclose(fileptr);
}

void printfile(void) {  

    // variable declarations
    FILE *fileptr;          // declare a file pointer
    PERSON tempRecord;      // declare PERSON variable to store temporary person record
                            // this will be reused as program iterates through each record it reads from the file
    
    // attempt to open a file named bin.dat in binary read mode ("rb")
    if((fileptr=fopen("persons.bin","rb")) == NULL) {

        // if the file opening fails, print an error message 
        printf("Cannot create the file\n");

        // exit the program with status code 1 denoting unsuccessful program execution
        exit(1);
    }

    // check if file is empty; if not, proceed with subroutine
    if (isEmpty(fileptr) == false) {

        // iterate through and read record until the end of the file
        // if fread successfully reads one PERSON element, it returns 1 and continues looping
        // if it fails to read an element (e.g. by reaching end of file), it returns 0 and exits the loop
        while (fread(&tempRecord, sizeof(PERSON), 1, fileptr) == 1) {

            // print divider line and current person data 
            printf("-----------------------------\n");
            printf("First Name: %s\n", tempRecord.firstname);
            printf("Last Name: %s\n", tempRecord.famname);
            printf("Personal Number: %s\n", tempRecord.pers_number);
        }
    }

    // close the file
    fclose(fileptr);
}