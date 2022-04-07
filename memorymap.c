#include <stdlib.h>  // rand(), srand()
#include <time.h>    // time()
#include <unistd.h>     /* fork() and getpid() */
#include <stdio.h>      /* printf() */
#include <string.h>
#include <sys/mman.h>   /* mmap()  */
#include <unistd.h>     /* fork() and getpid() */
#include <sys/wait.h>   /*wait*/
#include <errno.h>	    /* errno */

#define MSGSIZE 50

int main(int argc, char **argv) {
    
    	int n;
	    scanf("%d", &n); //input banyak proses
	    
        int pid;
        time_t t;
        int sum = 0;

        // /* Intializes random number generator */
        srand((unsigned) time(&t));

        printf("mulai \n");

        int size = MSGSIZE * sizeof(int);
        //buat memory map
        void *addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        printf("Mapped at %p\n", addr);
        int *shared = addr;

        switch (pid = fork()) {
        case 0:         /*  proses anak */
                printf("Proses anak \n");
                //isi data di memory map
                for(int i=0; i<n; i++){
                    shared[i] = rand() % 20; //mengisi angka random ke array
                    printf("producer %d write random number : %d \n", i+1, shared[i]);
                }
                break;
        default:        /*  ortu */
                printf("Proses ortu\n");
                int pidWait,status;
                /* wait sampai child selesai */
                while (pidWait = wait(&status)) {   
                    if (pidWait == pid)  /* child sukses selesai*/
                        //cetak isi memory map
                        for (int i = 0; i<n; i++) {
                          printf("consumer %d read random number : %d \n", i+1, shared[i]);
                          sum = sum + shared[i];
                        }
                        break; //keluar dari loop wait
                    if ((pidWait == -1) && (errno != EINTR)) {
                        /* ada error*/
                        perror("waitpid");
                        exit(1);
                    }
                }
                printf("sum from second group process: %d\n", sum); //cetak hasil sum
                break;
        case -1:        /* error */
                perror("fork");
                exit(1);
        }
        exit(0);
}
