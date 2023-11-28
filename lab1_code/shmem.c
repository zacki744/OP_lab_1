#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#define SHMSIZE 128
#define SHM_R 0400
#define SHM_W 0200
#define SIZE_OF_BUFFER 10


int main(int argc, char **argv)
{
    struct shm_struct {
        int buffer[SIZE_OF_BUFFER];
        int in;  // Index for producer
        int out; // Index for consumer
    };
	volatile struct shm_struct *shmp = NULL;
	char *addr = NULL;
	pid_t pid = -1;
	int var1 = 0, var2 = 0, shmid = -1;
	struct shmid_ds *shm_buf;

	/* allocate a chunk of shared memory */
	shmid = shmget(IPC_PRIVATE, SHMSIZE, IPC_CREAT | SHM_R | SHM_W);
	shmp = (struct shm_struct *) shmat(shmid, addr, 0);
    shmp->in = 0;
    shmp->out = 0;
	pid = fork();

	if (pid != 0) {
		/* here's the parent, acting as producer */
		while (var1 < 100) {
            // wait random time between 0.1s - 0.5s
            int r = rand() % 5 + 1;
            usleep(r * 100000);

			var1++;
			shmp->buffer[shmp->in] = var1; // assign the buffer acording to the incromented var1
			shmp->in = (shmp->in + 1) % SIZE_OF_BUFFER; // incriment the index for the input
			printf("Sending %d\n", var1); fflush(stdout);
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	} else {
		/* here's the child, acting as consumer */
		while (var2 < 100) {
			//designates a random number
			int r = rand() % (20) + 2;
			//sleap for a time
			sleep(r/10);

			var2 = shmp->buffer[shmp->out]; //assign the var 2 from the buffor according to the  out variable
			shmp->out = (shmp->out + 1) % SIZE_OF_BUFFER; // assign the indexposistion for the output buffer to lock att for the next look up
			printf("Received %d\n", var2); fflush(stdout); // output
		}
		shmdt(addr);
		shmctl(shmid, IPC_RMID, shm_buf);
	}
}