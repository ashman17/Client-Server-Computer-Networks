#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void makingStrings() {
    // 1. static allocation
    char str1[20];
    strcpy(str1, "Static Hello World");

    // 2. dynamic allocation
    char* str2;
    str2 = (char*) malloc(20*sizeof(char));
    strcpy(str2, "Dynamic Hello World");

    printf("%s\n", str1);
    printf("%s\n", str2);

    // free heap allocated data
    free(str2);
}

void makingStringArrays() {
    // 1. static allocation (size and number of strings known)
    char strArr1[2][10];
    strcpy(strArr1[0], "strArr1");

    // 2. dynamic allocation (pointer to array of pointers)
    char* strArr2[2];
    strArr2[0] = strdup("strArr2");                 // strdup = (malloc + strcpy)

    // 3. dynamic allocation (pointer to contiguous block of memeory)
    char** strArr3;
    strArr3 = (char**) malloc(2*sizeof(char*));
    strArr3[0] = strdup("strArr3");                 // strdup = (malloc + strcpy)

    printf("%s\n", strArr1[0]);
    printf("%s\n", strArr2[0]);
    printf("%s\n", strArr3[0]);

    free(strArr2[0]);
    free(strArr3);
}

void manipulateStrings() {

    // 1. strncpy
    char* str1;
    str1 = strdup("Hello");
    strncpy(str1, str1, 3);
}

int main (int argc, char* argv[]) {

    // makingStrings();
    // makingStringArrays();
    // manipulateStrings();

    char list[50];
    strcpy(list, "Static:Hello:World");
    char new_list[1024];

    char* name = strtok(strdup(list), ":");
    char temp[5];
    int count = 1;

    while (name) {
        sprintf(temp, "%d. ", count);
        strcat(new_list, temp);
        strcat(new_list, name);
        strcat(new_list, "\n");
        name = strtok(NULL, ":");
        count++;
    }

    printf("ORIGINAL LIST: %s\n", list);
    printf("NEW LIST: %s", new_list);
    fflush(stdout);

    return EXIT_SUCCESS;
}