#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define PROFESSOR_COUNT 5
// Shared Variables
pthread_mutex_t choppsticks[PROFESSOR_COUNT];
const char *professors[] = {"Tanenbaum", "Dijkstra", "Knuth", "Lamport", "Turing"};
int professor_eating_count[] = {0, 0, 0, 0, 0};
void thinking(int id) {
    //printf("%s: is thinking\n", professors[id]);
    //sleep((rand() % 4) + 1);
}
void thinkingTwo(int id) {
    //printf("%s: got left chopstick\n", professors[id]);
    //sleep((rand() % 6) + 2);
}
void eating(int id) {
    professor_eating_count[id]++;
    //printf("%s: is eating\n", professors[id]);
    printf("%s: %d\n", professors[id], professor_eating_count[id]);
    //sleep((rand() % 5) + 5);
}

void* DinnerTable(void* arg) {
    int id = *(int*)arg;

    while (1) {
        thinking(id);// first thinking phase
        pthread_mutex_lock(&choppsticks[id]); //lock the left chopstick of the pofessor
        thinkingTwo(id); // think after picking upp the left chopstick

        //designate the vars
        int left = id; //chippstick
        int right = (id + 1) % PROFESSOR_COUNT; //choppstick
        int check = pthread_mutex_trylock(&choppsticks[right]); //try

        if (check != 0) {
            //printf("%s: got right chopstick.\n", professors[left]);
            eating(id);
            pthread_mutex_unlock(&choppsticks[right]);
        }
        else {
            //printf("%s: cannot eat rigth now, putting down left chopstick.\n", professors[left]);
        }
        pthread_mutex_unlock(&choppsticks[left]);
    }
    return NULL;

}
int main() {
    pthread_t threads[PROFESSOR_COUNT];
    int ids[PROFESSOR_COUNT];

    srand(time(NULL));

    for (int i = 0; i < PROFESSOR_COUNT; i++) {
        pthread_mutex_init(&choppsticks[i], NULL);
    }

    for (int i = 0; i < PROFESSOR_COUNT; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, DinnerTable, &ids[i]);
    }

    for (int i = 0; i < PROFESSOR_COUNT; i++) {
        pthread_join(threads[i], NULL);
        pthread_mutex_destroy(&choppsticks[i]);
    }

    return 0;
}

/*
Mutual Exclusion: Resources cannot be shared. In this case, it means each philosopher must use a chopstick exclusively.

Hold and Wait: A process holds at least one resource and is waiting for additional resources that are currently held by other processes. In the Dining Philosophers problem, this can occur if a philosopher holds one chopstick and waits for the other one.

No Preemption: Resources cannot be forcibly taken away from the process holding them. This means that once a philosopher holds a chopstick, they won't release it until they finish eating.

Circular Wait: A circular chain of two or more processes, each holding a resource and waiting for a resource that's held by another process in the chain.*/