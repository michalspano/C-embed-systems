#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// typedefs
typedef struct {
    char firstname[20];
    char famname[20];
    char pers_number[13];   // yyyymmddnnnc
} PERSON;

// function declarations
void clearBuffer(void);

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
    bool entryFound;

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
            case '1':
                // dummy data to be stored as first entry in 
                ppost = (PERSON){"Duke", "Nukem", "199409021234"};

                write_new_file(&ppost);

                // exit the switch block after case is executed
                break;

            case '2':
                ppost = input_record();

                append_file(&ppost);

                // exit the switch block after case is executed
                break;

            case '3':
                do {
                    clearBuffer();

                    printf("Please type name to search: ");

                    //fgets
                    scanf("%19s", searchInput);

                    entryFound = search_by_name(searchInput);

                    if(!entryFound) {

                        clearBuffer();
                        printf("Try again? (Y/N): ");

                        searchChoice = getchar();
                        
                        while(searchChoice != 'N' && searchChoice != 'n' &&
                              searchChoice != 'Y' && searchChoice != 'y') {

                            printf("Invalid input. Please choose Y or N: ");

                            clearBuffer();
                            searchChoice = getchar();
                        }
                    }
                } while (searchChoice == 'Y' || searchChoice == 'y' && entryFound != true);
        
                // exit the switch block after case is executed
                break;

            case '4':
                printfile();

                // exit the switch block after case is executed
                break;

            case '5':
                printf("Thank you for using our program.\n");
                
                // exit the switch block after case is executed
                break;

            // if input is anything other than chars 1-5
            default:
                // print negative message
                printf("Invalid input.\n");
                
                // exit the switch block after case is executed
                break;
        }

    // ensure buffer isn't cleared if user input only contains a single \n
    // this is relevant when the user presses ENTER with no other characters
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

    // declare temporary character
    int c;

    // consume consecutive haracters from buffer in a loop until encountering \n or end of file
    while ((c = getchar()) != '\n' && c != EOF);
}

// create file and write the first record
// takes PERSON pointer containing dummy data
void write_new_file(PERSON *inrecord) {   
    FILE *fileptr;
    
    // attempt to open a file named persons.bin in binary write mode ("wb")
    if((fileptr=fopen("persons.bin","wb")) == NULL) {
        printf("Cannot create the file\n");
        exit(1);
    }

    fwrite(inrecord, sizeof(PERSON), 1, fileptr);

    fclose(fileptr);

    printf("File successfully created.\n");

}      

PERSON input_record(void) {
    PERSON tempRecord = (PERSON){};

    printf("Please enter first name: ");
    scanf("%19s", tempRecord.firstname);    //fgets
    clearBuffer();

    printf("Please enter family name: ");
    scanf("%19s", tempRecord.famname);  //fgets 
    clearBuffer();

    printf("Please enter personal number: ");
    scanf("%12s", tempRecord.pers_number);  //fgets

    return tempRecord;
};

void append_file(PERSON *inrecord) {
    FILE *fileptr;
    
    // attempt to open a file named persons.bin in binary append mode ("ab")
    if((fileptr=fopen("persons.bin","ab")) == NULL) {
        printf("Cannot create the file\n");
        exit(1);
    }

    fwrite(inrecord, sizeof(PERSON), 1, fileptr);

    fclose(fileptr);

    printf("Person successfully recorded in file.\n");
}

bool search_by_name(char *name) {
    FILE *fileptr;
    PERSON tempRecord;
    bool entryFound = false;

    // attempt to open a file named persons.bin in binary read mode ("rb")
    if((fileptr=fopen("persons.bin","rb")) == NULL) {
        printf("Cannot create the file\n");
        exit(1);
    }

    // check if the file is empty
    fseek(fileptr, 0, SEEK_END);

    if (ftell(fileptr) == 0) {
        printf("The file is empty. \n");

    } else {
        // move the file pointer to the beginning
        fseek(fileptr, 0, SEEK_SET);

        // read and print each record until the end of the file
        // if fread successfully reads one PERSON struct, it returns 1
        // if it fails to read an element (e.g. by reaching end of file), it returns 0
        while (fread(&tempRecord, sizeof(PERSON), 1, fileptr) == 1) {

            if (strcmp(tempRecord.firstname, name) == 0 || strcmp(tempRecord.famname, name) == 0) {
                entryFound = true;
                printf("-----------------------------\n");
                printf("First Name: %s\n", tempRecord.firstname);
                printf("Last Name: %s\n", tempRecord.famname);
                printf("Personal Number: %s\n", tempRecord.pers_number);
            }
        }

        if(entryFound == false) {
            printf("No matching entries found.\n");
            return false;
        }
        return true;
    }

    fclose(fileptr);
}

void printfile(void) {  
    FILE *fileptr;
    PERSON tempRecord;
    
    // attempt to open a file named bin.dat in binary read mode ("rb")
    if((fileptr=fopen("persons.bin","rb")) == NULL) {
        printf("Cannot create the file\n");
        exit(1);
    }

    // check if the file is empty
    fseek(fileptr, 0, SEEK_END);

    if (ftell(fileptr) == 0) {
        printf("The file is empty. \n");

    } else {
        // move the file pointer to the beginning
        fseek(fileptr, 0, SEEK_SET);

        // read and print each record until the end of the file
        // if fread successfully reads one PERSON struct, it returns 1
        // if it fails to read an element (e.g. by reaching end of file), it returns 0
        while (fread(&tempRecord, sizeof(PERSON), 1, fileptr) == 1) {
            printf("-----------------------------\n");
            printf("First Name: %s\n", tempRecord.firstname);
            printf("Last Name: %s\n", tempRecord.famname);
            printf("Personal Number: %s\n", tempRecord.pers_number);
            
        }
    }

    fclose(fileptr);
}