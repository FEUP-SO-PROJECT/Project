#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CYPHER_PATH "cypher.txt"
#define BUF_SIZE 30
#define CHUNK_SIZE (BUF_SIZE - 1)
#define MAX_CYPHER 25
#define MAX_CYPHER_LENGTH 25
#define QUOTE_SIZE BUF_SIZE * 10



void separate_cyphers(char *array, int size,char **cypher_a, char **cypher_b){
    char* split = strtok(array," ");
    bool flag = true;
    int counter = 0;

    while(split != NULL)
    {
        if(flag){
            strcpy(cypher_a[counter],split);
            flag = false;
        }     
        else{
            strcpy(cypher_b[counter],split);
            flag = true;
            counter++;
        }
        split=strtok(NULL," ");
    }
}

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
                buf[i] = ' ';
            }
            file_content = insert_array(file_content,&size,index_counter,buf[i]);
            index_counter++;
        }
    }
    file_content[index_counter] = '\0';

    separate_cyphers(file_content,size,cypher_a,cypher_b);
    
    free(file_content);
    free(buf);

}

char* read_quote(){
    
    //allocate memory for quote;
    char* quote = (char*) malloc(QUOTE_SIZE * sizeof(char));
    int quote_size = (QUOTE_SIZE*sizeof(char));
    int quote_index = 0;

    char buf[BUF_SIZE];
    while (fgets(buf,CHUNK_SIZE,stdin) != NULL)
    {
        for(int i = 0; i < strlen(buf); i++){
            insert_array(quote,&quote_size,quote_index,buf[i]);
            quote_index++;
        }
    }
    return quote;
}


int main(int argc, char const *argv[])
{

    char **cypher_a,**cypher_b,*quote;

    //allocate memory for cypher_a and cypher_b;
    cypher_a = (char**) malloc(MAX_CYPHER * sizeof(*cypher_a));
    cypher_b = (char**) malloc(MAX_CYPHER * sizeof(*cypher_b));
    for(int i = 0; i < MAX_CYPHER; i++){
        cypher_a[i] = (char*) malloc(MAX_CYPHER_LENGTH * sizeof(*cypher_a[i]));
        cypher_b[i] = (char*) malloc(MAX_CYPHER_LENGTH * sizeof(*cypher_b[i]));
    }

    //read from cypher file
    read_cypher(cypher_a,cypher_b);

    quote = read_quote();

    printf("%s",quote);

    //Free allocated memory
    for(int i = 0; i < MAX_CYPHER; i++){
        free(cypher_a[i]);
        free(cypher_b[i]);
    }
    free(cypher_a);
    free(cypher_b);

    free(quote);

    return 0;
}
