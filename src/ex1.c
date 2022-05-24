#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
int main(int argc, char const *argv[])
{
#define BUF_SIZE 256
#define CHUNK_SIZE (BUF_SIZE - 1)

    // auxiliar buffer for storing chunks of text from the file
    char buf[BUF_SIZE];
    const char *filename = argv[1];
    FILE *fptr = fopen(filename, "r");

    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(0);
    }
    int a = 2;
    printf("[1] ");
    while (!feof(fptr))
    {
        int count;
        count = fread(buf, sizeof(char), CHUNK_SIZE, fptr);
        if (ferror(fptr))
            exit(0);
        buf[count] = '\0';
        for (int i = 0; i < strlen(buf); i++) {
            if (buf[i] == '.' || buf[i] == '!' || buf[i] == '?'){
                if (i + 1 == strlen(buf)) {
                    printf("%c",buf[i]);
                    break;
                }
                printf("%c\n[%i] ",buf[i] ,a);
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
                printf("%c",buf[i]);
            }
        }
    }
    printf("\n");
    fclose(fptr);
    return 0;
}