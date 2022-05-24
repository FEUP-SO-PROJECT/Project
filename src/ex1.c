#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
int main(int argc, char const *argv[])
{
    int n;
    if (argc == 1){
        printf("usage: phrases [-l] file\n");
        return 1;
    }
    else if(argc == 2){
        n = 1;
    }
    else if(argc == 3){
        if (strcmp(argv[1], "-1") == 0){
            n = 2;
        }
        else {
            printf("Wrong input\n");
            return 1;
        }
        
    }
    
#define BUF_SIZE 256
#define CHUNK_SIZE (BUF_SIZE - 1)

    // auxiliar buffer for storing chunks of text from the file
    char buf[BUF_SIZE];
    const char *filename = argv[n];
    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    int a = 1;
    if (n == 2){
        printf("[1] ");
    }
    while (!feof(fptr))
    {
        int count;
        count = fread(buf, sizeof(char), CHUNK_SIZE, fptr);
        if (ferror(fptr))
            exit(0);
        buf[count] = '\0';
        int check = 0;
        for (int i = 0; i < strlen(buf); i++) {
            if (check == 1 && n ==2){
                printf("\n[%i] ",a);
            }
            check = 0;
            if (buf[i] == '.' || buf[i] == '!' || buf[i] == '?'){
                if (i + 1 == strlen(buf)) {
                    if (n == 2){
                        printf("%c",buf[i]);
                    }
                    break;
                }
                check = 1;
                if (n == 2){
                    printf("%c",buf[i]);
                }
                a++;
                i++;
                while (buf[i + 1] == ' '){
                    i++;
                }
            }
            else if (buf[i] == '\n'){
                continue;
            }
            else {
                if (n == 2){
                    printf("%c",buf[i]);
                }
            }
        }
    }
    a--;
    if (n == 1){
        printf("%i",a);
    }
    printf("\n");
    fclose(fptr);
    return 0;
}