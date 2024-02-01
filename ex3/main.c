#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// typedefs
typedef struct {
    char firstname[20];
    char famname[20];
    char pers_number[13]; // yyyymmddnnnc
} PERSON;

// function declarations
void clearBuffer(void);

void write_new_file(PERSON *inrecord);              // creates file and writes the first record
PERSON input_record(void);                          // reads a person's record
void append_file(PERSON *inrecord);                 // appends a new person to the file
void search_by_name(char searchChoice, char *name); // prints out the person if in list based on first name attribute
void search_by_famname(char *name);                 // prints out the person if in list based on last name attribute
void printfile(void);                               // prints out all persons in the file

int main(void) {
    PERSON ppost;
    char taskChoice;
    char searchChoice;
    char searchInput[20];

    printf("-----------------------------\n");
    printf("Welcome to Exercise 3! \n");
    printf("1. Create a new and delete the old file.\n");
    printf("2. Add a new person to the file.\n");
    printf("3. Search for a person in the file.\n");
    printf("4. Print out all in the file.\n");
    printf("5. Exit the program.\n");

    do {
        printf("-----------------------------\n");
        printf("Please choose an option between 1 and 5: ");

        taskChoice = getchar();

        switch(taskChoice) {
            case '1':
                ppost = (PERSON){"Duke", "Nukem", "199409021234"};

                write_new_file(&ppost);

                break;

            case '2':
                ppost = input_record();

                append_file(&ppost);

                break;

            case '3':
                clearBuffer();
                
                printf("Search by first name or family name?\n");
                printf("1. First name.\n");
                printf("2. Family name.\n");

                printf("Please type your choice: ");

                searchChoice = getchar();

                while(searchChoice != '1' && searchChoice != '2') {
                    printf("Invalid input. Please choose between 1 or 2: ");

                    clearBuffer();
                    searchChoice = getchar();
                }

                printf("Please type name: ");

                scanf("%19s", searchInput);

                search_by_name(searchChoice, searchInput);

                break;

            case '4':
                printfile();

                break;

            case '5':
                printf("Thank you for using our program.\n");
                
                break;

            default:
                printf("Invalid input.\n");
                
                break;
        }

    if(taskChoice != '\n') {
        clearBuffer();
    }
    
    } while(taskChoice != '5');

    return 0;
}


void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void write_new_file(PERSON *inrecord) {     // creates file and writes the first record 
    FILE *fileptr;
    
    // attempt to open a file named bin.dat in binary write mode ("wb")
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
    scanf("%19s", tempRecord.firstname);
    clearBuffer();

    printf("Please enter second name: ");
    scanf("%19s", tempRecord.famname);
    clearBuffer();

    printf("Please enter personal number: ");
    scanf("%12s", tempRecord.pers_number);

    return tempRecord;
};

void append_file(PERSON *inrecord) {
    FILE *fileptr;
    
    // attempt to open a file named bin.dat in binary append mode ("ab")
    if((fileptr=fopen("persons.bin","a+b")) == NULL) {
        printf("Cannot create the file\n");
        exit(1);
    }

    fwrite(inrecord, sizeof(PERSON), 1, fileptr);

    fclose(fileptr);

    printf("Person successfully recorded in file.\n");
}

void search_by_name(char searchChoice, char *name) {
    FILE *fileptr;
    PERSON tempRecord;
    bool entryFound = false;

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

            if (searchChoice == '1' && strcmp(tempRecord.firstname, name) == 0 ||
                searchChoice == '2' && strcmp(tempRecord.famname, name) == 0) {
                entryFound = true;
                printf("-----------------------------\n");
                printf("First Name: %s\n", tempRecord.firstname);
                printf("Last Name: %s\n", tempRecord.famname);
                printf("Personal Number: %s\n", tempRecord.pers_number);
            }
        }

        if(entryFound == false) {
            printf("No matching entries found.\n");
        }
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