#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
    pid_t pid;
    pid_t pid2;
    unsigned i;
    unsigned niterations = 100;
    
    pid = fork();

    if (pid == 0) {
        // This code is executed in the first child process
        for (i = 0; i < niterations; ++i)
            printf("A = %d, ", i);
    } else {
        // Parent process
        for (i = 0; i < niterations; ++i)
            printf("B = %d, ", i);

        // create therd thread
        pid2 = fork(); 
        printf("\n");


        if (pid2 == 0) {
            // This code is executed in the therd child process
            for (i = 0; i < niterations; ++i)
                printf("C = %d, ", i);
        }
        else {
            printf("\n");
            printf("%d", pid);
            printf("\n");
            //wait for parrent pid
            waitpid(pid, NULL, 0);
            //wait for parent proces pid 2
            waitpid(pid2,NULL, 0);
        }
    }
    printf("\n");
    return 0;
}
