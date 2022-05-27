#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


#define CYPHER_PATH "cypher.txt"
#define BUF_SIZE 256
#define CHUNK_SIZE (BUF_SIZE - 1)
#define MAX_CYPHER 25
#define MAX_CYPHER_LENGTH 25
#define QUOTE_SIZE BUF_SIZE * 10
#define CYPHER_GAP 100
#define CYPHER_WORDS 100
#define LINESIZE 4096
#define READ_END 0
#define WRITE_END 1



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

    char **cypher_a,**cypher_b,*quote,**cyphered,line[LINESIZE],cyphered_line[LINESIZE];
    int size,nbytes, fd1[2],fd2[2];
    pid_t pid;

    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    else if (pid > 0) {
        /* parent */
        //read quote from stdin
        quote = read_quote();

        //process management
        close(fd1[READ_END]);
        snprintf(line, strlen(quote), "%s", quote);
        if ((nbytes = write(fd1[WRITE_END], line, strlen(line))) < 0) {
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(fd1[WRITE_END]);
        /* wait for child and exit */
        if ( waitpid(pid, NULL, 0) < 0) {
            fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        //If the program reaches this point, the child process has received and cyphered the input.
        close(fd2[WRITE_END]);
        if ((nbytes = read(fd2[READ_END], cyphered_line, LINESIZE)) < 0 ) {
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(fd2[READ_END]);

        write(STDOUT_FILENO,cyphered_line,strlen(cyphered_line));

        //Free allocated memory for quote
        free(quote);

        exit(EXIT_SUCCESS);
    }
    else {
        /* child */
        close(fd1[WRITE_END]);
        if ((nbytes = read(fd1[READ_END], line, LINESIZE)) < 0 ) {
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(fd1[READ_END]);
        /* write message from parent */
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

        size = encrypt_cypher(line,cypher_a,cypher_b,cypher_size,cyphered);

        int index = 0;

        for(int i = 0 ; i < size; i++){
            for(int j = 0; j < strlen(cyphered[i]); j++){
                cyphered_line[index] = cyphered[i][j];
                index++;
            }
            if(i != size-1) cyphered_line[index] = ' ';
            index++;
        }
        //Send data to parent process
        close(fd2[READ_END]);
        if ((nbytes = write(fd2[WRITE_END], cyphered_line, strlen(cyphered_line))) < 0) {
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        close(fd2[WRITE_END]);

        //Free allocated memory for cyphered
        for(int i = 0; i < CYPHER_WORDS; i++){
            free(cyphered[i]);
        }
        free(cyphered);

        //Free allocated memory for cypher_a and cypher_b
        for(int i = 0; i < MAX_CYPHER; i++){
            free(cypher_a[i]);
            free(cypher_b[i]);
        }
        free(cypher_a);
        free(cypher_b);

        /* exit gracefully */
        exit(EXIT_SUCCESS);
    }
    return 0;
}
