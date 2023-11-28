/***************************************************************************
 *
 * Sequential version of Matrix-Matrix multiplication
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1024

struct RowRes {
    int start_row;
    int end_row;
};
static double a[SIZE][SIZE];
static double b[SIZE][SIZE];
static double c[SIZE][SIZE];

static void *
init_matrix(void* args)
{
    struct RowRes *res = (struct RowRes *)args;
    for (int i = res->start_row; i < res->end_row; i++) {
        for (int j = 0; j < SIZE; j++) {
            /* Simple initialization, which enables us to easy check
                * the correct answer. Each element in c will have the same
                * value as SIZE after the matmul operation.
                */
            a[i][j] = 1.0;
            b[i][j] = 1.0;
        }
    }
}

static void *
matmul_seq(void* args)
{
    struct RowRes *data = (struct RowRes *)args;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < SIZE; j++) {
            c[i][j] = 0.0;
            for (int k = 0; k < SIZE; k++)
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
    }
}

static void
print_matrix(void)
{
    int i, j;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++)
	        printf(" %7.2f", c[i][j]);
	    printf("\n");
    }
}

int
main(int argc, char **argv)
{
     if (argc != 2) {
        printf("Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    int rows_per_thread = SIZE / num_threads;
    //printf("rows_per_thread: %d\n", rows_per_thread);

    pthread_t init_threads[num_threads];
    struct RowRes init_thread_data[num_threads];

    for (int i = 0; i < num_threads; i++) {
        init_thread_data[i].start_row = i * rows_per_thread;
        init_thread_data[i].end_row = (i + 1) * rows_per_thread;
        pthread_create(&init_threads[i], NULL, init_matrix, &init_thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(init_threads[i], NULL);
    }

    pthread_t mul_threads[num_threads];
    struct RowRes mul_thread_data[num_threads];

    for (int i = 0; i < num_threads; i++) {
        mul_thread_data[i].start_row = i * rows_per_thread;
        mul_thread_data[i].end_row = (i + 1) * rows_per_thread;
        pthread_create(&mul_threads[i], NULL, matmul_seq, &mul_thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(mul_threads[i], NULL);
    }

    return 0;
}
