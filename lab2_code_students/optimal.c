#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAGES 100000

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s no_phys_pages page_size filename\n", argv[0]);
        return 1;
    }

    int no_phys_pages = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = argv[3];

    printf("No physical pages = %d, page size = %d\n", no_phys_pages, page_size);

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    int mem_ref[MAX_PAGES];
    int i = 0;
    while (fscanf(fp, "%d", &mem_ref[i]) != EOF) {
        i++;
    }
    fclose(fp);

    int num_mem_refs = i;
    printf("Reading memory trace from %s... Read %d memory references\n", filename, num_mem_refs);

    int page_table[MAX_PAGES];
    memset(page_table, -1, sizeof(page_table));

    int page_faults = 0;
    for (i = 0; i < num_mem_refs; i++) {
        int page_num = mem_ref[i] / page_size;
        int j;
        for (j = 0; j < no_phys_pages; j++) {
            if (page_table[j] == page_num) {
                break;
            }
        }
        if (j == no_phys_pages) {
            int k;
            int max_future = -1;
            int max_future_page = -1;
            for (k = 0; k < no_phys_pages; k++) {
                int l;
                for (l = i + 1; l < num_mem_refs; l++) {
                    if (mem_ref[l] / page_size == page_table[k]) {
                        if (l > max_future) {
                            max_future = l;
                            max_future_page = k;
                        }
                        break;
                    }
                }
                if (l == num_mem_refs) {
                    max_future_page = k;
                    break;
                }
            }
            page_table[max_future_page] = page_num;
            page_faults++;
        }
    }

    printf("Result: %d page faults\n", page_faults);

    return 0;
}
