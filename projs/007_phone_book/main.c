#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_LEN 100

typedef struct  {
    // int sn;
    char name[STR_LEN];
    char number[14];
} PhoneAddress;

const char *Book = "phone-book.txt";

const char *PHONEBOOK_OUT = "{\"name\": \"%[^\"]\", \"number\": \"%[^\"]\"}\n";
const char *PHONEBOOK_IN = "{\"name\": \"%s\", \"number\": \"%s\"}\n";

int write_to_book(PhoneAddress *phoneAddr);
int read_from_book(void);

int main(int argc, char **argv){
    (void) argc; (void) argv;

    PhoneAddress first = {.name="SOME FICTIONAL NAME", .number="9742-0987554"};

    int writeErr = write_to_book(&first);    
    if(writeErr) {
        printf("Error Writing To File\n ");
        return EXIT_FAILURE;
    }

    int err = read_from_book();
    if(err) {
        printf("Error Reading from File\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int write_to_book(PhoneAddress *phoneAddr) {
    FILE *fp = fopen(Book, "a+");
    if(!fp) {
        perror("Error Opening File \n");
        return EXIT_FAILURE;
    }

    fprintf(fp, PHONEBOOK_IN, phoneAddr->name, phoneAddr->number);
    fclose(fp);
    return EXIT_SUCCESS;
}

int read_from_book(void){
    FILE *fp = fopen(Book, "r");
    if(!fp) {
        perror("Could not open file");
        return EXIT_FAILURE;
    }

    PhoneAddress address[10];

    char name[STR_LEN];
    char number[14];

    int i = 0;
    // while(fscanf(fp, PHONEBOOK_OUT, name, number) != EOF) {
    //     strcpy(address[i].name, name);
    //     strcpy(address[i].number, number);
    //     i++;

    //     printf("READ name = %s \t number = %s\n", address[i].name, address[i].number);
    // }
    while (i < 10 && fscanf(fp, PHONEBOOK_OUT, name, number) == 2) {
        strncpy(address[i].name, name, STR_LEN - 1);
        address[i].name[STR_LEN - 1] = '\0'; // Ensure null-termination
        strncpy(address[i].number, number, 13);
        address[i].number[13] = '\0'; // Ensure null-termination
        printf("READ name = %s \t number = %s\n", address[i].name, address[i].number);
        i++;
    }

    fclose(fp);
    return EXIT_SUCCESS;
}