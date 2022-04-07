
#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>

int main(){
	
    int n;
    scanf("%d", &n); //input banyak proses

    int fd[2]; //pipe

    int pid;
    time_t t;
    int sum = 0;

    // /* Intializes random number generator */
    srand((unsigned) time(&t));

    //create pipe
    if (pipe(fd) < 0) {
        exit(1); //error
    }

    //buat array
    int writeNumber[n];
    int readNumber[n];


    printf("mulai \n");

        switch (pid = fork()) {

        case 0:         /* fork returns 0 ke proses anak */
            printf("Proses anak \n");
            /* tutup bagian input dari pipe */
            close(fd[0]);
                // tulis data ke pipe
            for(int i=0; i<n; i++){
                writeNumber[i] = rand() % 20; //get random number
                write(fd[1], &writeNumber[i], sizeof(writeNumber[i])); //menulis ke pipe
                printf("Proucer %d in the first group process write random number : %d\n", i, writeNumber[i]); //print random number 
            }
            break;

        default:        /* fork returns pid ke proses ortu */
            printf("Proses ortu\n");
            /* tutup bagian output dari pipe */
            close(fd[1]);
            // baca yang ditulis child dari pipe
            for(int i=0; i<n; i++){
                read(fd[0], &readNumber[i], sizeof(readNumber[i]));
                printf("Consemer int the second group process read random number : %d\n", readNumber[i]); //print random number
                sum = sum + readNumber[i]; //jumlahkan 
            }
            printf("sum from second group process: %d\n", sum);

            break;
            
        case -1:        /* error */
            perror("fork");
            exit(1);
        }
        
        exit(0);
}
