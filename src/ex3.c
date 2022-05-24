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
#define CYPHER_GAP 100
#define CYPHER_WORDS 100



int separate_cyphers(char *array, int size,char **cypher_a, char **cypher_b){
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
    return counter;
}

int split_string(char *array, char *delim, char** split_arr){
    char* split = strtok(array,delim);
    int counter = 0;

    while(split != NULL){
        strcpy(split_arr[counter],split);
        split=strtok(NULL," ");
        counter++;
    }
    return counter;
}

char* insert_array(char *array,int *size,int index,char value){
    if(index >= *size){
        *size = *size + BUF_SIZE;
        array = (char*) realloc(array,(*size)*sizeof(char));
    }
    array[index] = value;
    return array;
}

int read_cypher(char** cypher_a, char** cypher_b){
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

    int counter = separate_cyphers(file_content,size,cypher_a,cypher_b);
    
    free(file_content);
    free(buf);

    return counter;
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
    quote[quote_index] = '\0';
    return quote;
}


int encrypt_cypher(char *quote, char **cypher_a,char **cypher_b,int cypher_size, char **cyphered){
    int size;
    char *pointer;
    char stringToReplace[CYPHER_GAP];
    char stringToCat[CYPHER_GAP/5];

    //splits quote into cyphered multidimensional array cyphered.
    size = split_string(quote," ",cyphered);

    for(int i = 0; i < size; i++){
        for(int j = 0; j < cypher_size; j++){
            //verify if cypher_a[i] is the current word and replace it with its respective cypher
            pointer = strstr(cyphered[i],cypher_a[j]);
            if(pointer != NULL){

                strcpy(stringToReplace,cypher_b[j]);

                int dif = strlen(cyphered[i]) - strlen(cypher_a[j]);

                if(dif > 0){
                    for(int k = 0; k < dif; k++){
                        stringToCat[k] = cyphered[i][k+strlen(cypher_a[j])];
                    }
                }
                stringToCat[dif] = '\0';

                strcat(stringToReplace,stringToCat);
                strcpy(pointer,stringToReplace);
                break;
            }
            //verify if cypher_b[i] is the current word and replace it with its respective cypher
            pointer = strstr(cyphered[i],cypher_b[j]);
            if(pointer != NULL){
                strcpy(stringToReplace,cypher_a[j]);

                int dif = strlen(cyphered[i]) - strlen(cypher_b[j]);

                if(dif > 0){
                    for(int k = 0; k < dif; k++){
                        stringToCat[k] = cyphered[i][k+strlen(cypher_b[j])];
                    }
                }
                stringToCat[dif] = '\0';

                strcat(stringToReplace,stringToCat);
                strcpy(pointer,stringToReplace);
                break;
            }
        }
    }
    
    return size;
}


int main(int argc, char const *argv[])
{

    char **cypher_a,**cypher_b,*quote,**cyphered;
    int size; 


    //allocate memory for cypher_a and cypher_b;
    cypher_a = (char**) malloc(MAX_CYPHER * sizeof(*cypher_a));
    cypher_b = (char**) malloc(MAX_CYPHER * sizeof(*cypher_b));
    for(int i = 0; i < MAX_CYPHER; i++){
        cypher_a[i] = (char*) malloc(MAX_CYPHER_LENGTH * sizeof(*cypher_a[i]));
        cypher_b[i] = (char*) malloc(MAX_CYPHER_LENGTH * sizeof(*cypher_b[i]));
    }

    //allocating memory for cyphered
    cyphered = (char**) malloc(CYPHER_WORDS*sizeof(char*));
    for(int i = 0; i < CYPHER_WORDS; i++){
        cyphered[i] = (char*) malloc(BUF_SIZE * 2 * sizeof(char));
    }

    //read from cypher file
    int cypher_size = read_cypher(cypher_a,cypher_b);

    //read quote from stdin
    quote = read_quote();

    size = encrypt_cypher(quote,cypher_a,cypher_b,cypher_size,cyphered);
    
    
    for(int i = 0 ; i < size; i++){
        printf("%s ",cyphered[i]);
    }
    

    //Free allocated memory for cypher_a and cypher_b
    for(int i = 0; i < MAX_CYPHER; i++){
        free(cypher_a[i]);
        free(cypher_b[i]);
    }
    free(cypher_a);
    free(cypher_b);

    //Free allocated memory for cyphered
    for(int i = 0; i < CYPHER_WORDS; i++){
       free(cyphered[i]);
    }
    free(cyphered);

    //Free allocated memory for quote
    free(quote);

    return 0;
}
