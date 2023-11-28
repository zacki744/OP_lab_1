#include <stdio.h>
#include <stdlib.h>

#define MAX 100000

int main(int argc, char *argv[]) {
    int no_phys_pages = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = argv[3];

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file\n");
        return 1;
    }

    int *pages = (int *)calloc(no_phys_pages, sizeof(int));
    int *memory_trace = (int *)malloc(MAX * sizeof(int));
    int i, j, k, flag, faults = 0, count = 0;

    for (i = 0; i < MAX; i++) {
        fscanf(file, "%d", &memory_trace[i]);
        memory_trace[i] /= page_size;
    }

    for (i = 0; i < MAX; i++) {
        flag = 0;
        for (j = 0; j < no_phys_pages; j++) {
            if (pages[j] == memory_trace[i]) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            pages[count % no_phys_pages] = memory_trace[i];
            count++;
            faults++;
        }
    }

    printf("No physical pages = %d, page size = %d\n", no_phys_pages, page_size);
    printf("Reading memory trace from %s... Read %d memory references\n", filename, MAX);
    printf("Result: %d page faults\n", faults);

    free(pages);
    free(memory_trace);
    fclose(file);

    return 0;
}
