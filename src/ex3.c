#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CYPHER_PATH "cypher.txt"
#define BUF_SIZE 256
#define MAX_CYPHER 25
#define MAX_CYPHER_LENGTH 25
#define CHUNK_SIZE (BUF_SIZE - 1)

void read_cypher(char** cypher_a, char** cypher_b){
    FILE *fptr = fopen(CYPHER_PATH, "r");
    char buf[BUF_SIZE];

    //Cannot open file
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(1);
    }

    //reading file by chunks
    while (!feof(fptr))
    {
        int count;
        count = fread(buf, sizeof(char), CHUNK_SIZE, fptr);
        if (ferror(fptr))
            exit(0);
        
        //iterating through buffer
        for (int i = 0; i < count; i++) {
            if(buf[i] == '\n'){
                continue;
            }
            printf("%c",buf[i]);        
        }
        printf("\n");
    }
}


int main(int argc, char const *argv[])
{

    char **cypher_a,**cypher_b;

    //allocate memory for cypher_a and cypher_b;
    cypher_a = malloc(MAX_CYPHER * sizeof(*cypher_a));
    cypher_b = malloc(MAX_CYPHER * sizeof(*cypher_b));
    for(int i = 0; i < MAX_CYPHER; i++){
        cypher_a[i] = malloc(MAX_CYPHER_LENGTH * sizeof(*cypher_a[i]));
        cypher_b[i] = malloc(MAX_CYPHER_LENGTH * sizeof(*cypher_b[i]));
    }

    //read from cypher file
    read_cypher(cypher_a,cypher_b);

    //Free allocated memory
    for(int i = 0; i < MAX_CYPHER; i++){
        free(cypher_a[i]);
        free(cypher_b[i]);
    }
    free(cypher_a);
    free(cypher_b);

    return 0;
}
