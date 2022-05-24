#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CYPHER_PATH "cypher.txt"
#define BUF_SIZE 256
#define CHUNK_SIZE (BUF_SIZE - 1)
#define MAX_CYPHER 25
#define MAX_CYPHER_LENGTH 25



char* insert_array(char *array,int *size,int index,char value){
    if(index >= *size){
        *size = *size + BUF_SIZE;
        array = (char*) realloc(array,(*size)*sizeof(char));
    }
    array[index] = value;
    return array;
}


void read_cypher(char** cypher_a, char** cypher_b){
    FILE *fptr = fopen(CYPHER_PATH, "r");

    int index_counter = 0;
    int size = BUF_SIZE*5;
    char *file_content = (char*) malloc(size*sizeof(char));
    char *buf = (char*) malloc(BUF_SIZE*sizeof(char));

    //Cannot open file
    if (fptr == NULL)
    {
        printf("Cannot open file \n");
        exit(1);
    }

    /*
    while(fgets(buf,BUF_SIZE,fptr)){
        for(int i = 0; i < strlen(buf);i++){
            if(buf[i] != '\n') printf("%c",buf[i]);
        }
    }
    */

    
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
            file_content = insert_array(file_content,&size,index_counter,buf[i]);
            index_counter++;
        }
    }
    file_content[index_counter] = '\0';
    
    for(int i = 0; i < index_counter; i++){
        printf("%c",file_content[i]);
    }
    printf("\n");

    free(file_content);
    free(buf);
}


int main(int argc, char const *argv[])
{

    char **cypher_a,**cypher_b;

    //allocate memory for cypher_a and cypher_b;
    cypher_a = (char**) malloc(MAX_CYPHER * sizeof(*cypher_a));
    cypher_b = (char**) malloc(MAX_CYPHER * sizeof(*cypher_b));
    for(int i = 0; i < MAX_CYPHER; i++){
        cypher_a[i] = (char*) malloc(MAX_CYPHER_LENGTH * sizeof(*cypher_a[i]));
        cypher_b[i] = (char*) malloc(MAX_CYPHER_LENGTH * sizeof(*cypher_b[i]));
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
