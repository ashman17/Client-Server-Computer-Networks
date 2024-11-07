#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[])
{
    char arr[] = "P;CSE=1;EEE=2;|G;CHEM=3;";
    
    char* token1 = strtok(arr, "|");
    char* token2 = strdup(token1);
    token1 = strtok(NULL, "|");

    printf("%s\n%s\n", token1, token2);
    

    return 0;
}