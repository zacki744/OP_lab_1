#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100000

int main(int argc, char *argv[]) {
    //read args
    int no_phys_pages = atoi(argv[1]);
    int page_size = atoi(argv[2]);
    char *filename = argv[3];
    //initiate the list
    int mem_ref[MAX];
    int page_table[MAX];
    //allocate
    memset(page_table, -1, sizeof(page_table));

    //read mem from file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }
    int i = 0;
    while (fscanf(fp, "%d", &mem_ref[i]) != EOF) {
        i++;
    }
    fclose(fp);
    int n = i;

    // optimal algorithem
    int page_faults = 0;
    for (i = 0; i < n; i++) {
        int page_num = mem_ref[i] / page_size;
        int j;
        for (j = 0; j < no_phys_pages; j++) {
            if (page_table[j] == page_num) {
                break;
            }
        }
        if (j == no_phys_pages) { // page fault handeling
            int k;
            int max_future = -1;
            int max_future_page = -1;
            for (k = 0; k < no_phys_pages; k++) {
                int l;
                for (l = i + 1; l < n; l++) {
                    if (mem_ref[l] / page_size == page_table[k]) {
                        if (l > max_future) {
                            max_future = l;
                            max_future_page = k;
                        }
                        break;
                    }
                }
                if (l == n) {
                    max_future_page = k;
                    break;
                }
            }
            page_table[max_future_page] = page_num; // assign new page
            page_faults++; // incriment page faults
        }
    }
    // print 
    printf("No physical pages = %d, page size = %d\n", no_phys_pages, page_size);
    printf("Reading memory trace from %s...\nRead %d memory references\n", filename, n);
    printf("Result: %d page faults\n", page_faults);
    return 0;
}
