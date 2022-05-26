#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

    /* check if exactly three arguments are present */
    if (argc != 3)
    {
        printf("usage: addmx file1 file2\n");
        return EXIT_FAILURE;
    }
    const char *filename1 = argv[1];
    const char *filename2 = argv[2];
    FILE *file1 = fopen (filename1, "r");
    FILE *file2 = fopen (filename2, "r");
    

   // int nproc = j;

    if(file1 == NULL || file2 == NULL){
        printf("Cannot open file\n");
        return 1;
    }
    int x, y, n, m;
    for (int i = 0; i < 2; i++){
        fscanf(file1, "%d", &x);
        //printf("%i", x);
        fscanf(file2, "%d", &y);
        //printf("%i", y);
        if (i == 0){
            if (fgetc(file1) != 'x' || fgetc(file2)!='x'){
                printf ("file with different dimentions\n");
                return 1;
            }
            n = x;
        }
        else if(i == 1){
            if (fgetc(file1) != '\n' || fgetc(file2) != '\n'){
                printf ("file with different dimentions\n");
                return 1;
            }
            m = x;
        }
        if(x != y){
            printf ("file with different dimentions\n");
            return 1;
        }
    }

    int matrix1[n*m], matrix2[n*m];

    for (int i = 0; i < n * m; i++){
        
            fscanf(file1, "%d", &x);
            fscanf(file2, "%d", &y);
            matrix1[i] = x;
            matrix2[i] = y;
        
    }

    /*for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            printf("%d\n",matrix1[i][j]);
        }
    }
    printf("\n");
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            printf("%d\n",matrix2[i][j]);
        }
    }*/


//shared memmory
    int *partials = mmap(NULL, n*m*sizeof(int)*3, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, 0, 0);
        if(partials == MAP_FAILED){
            perror("mmap");
            exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n*m; i++){
        partials[i] = matrix1[i];
        partials[i + m*n] = matrix2[i];
        partials[i + m*n*2] = 0;
    }

    /*for (int i = 0; i < n*m*3; i++){
        printf("%d\n", partials[i]);
    }*/
    /* ------ start procs and do work ------ */
    for (int h = 0; h < m; h++){
        if (fork() == 0){
            for (int i = 0; i < n; i++){
                partials[i + m*n*2 + n * h] = partials[i + n * h] + partials[i + m*n + n * h];
            }
            //printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
            exit(0);
        }
    }
    /* ------ wait for procs to finish ------- */
    for (int k = 0; k < m; k++){
        wait(NULL);
    }

    


     /*ler resultados enviados pelos processos filhos */
    int p;
    int count = 0;
    for (p = n*m*2; p < n*m*3; p++){
        count++;
        if (p == n*m*2){
            printf("%ix%i\n", n, m);
        }
        printf("%d ", partials[p]);
        if (count % m == 0){
            printf("\n");
        }
    }


    if (munmap(partials, sizeof(partials)) < 0){
        perror("nunmap");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
