// A C program to implement LRU page replacement algorithm
#include <stdio.h>
#include <stdlib.h>
#define MAX 100000

// A function to find the page with the minimum index in the frame
int findMin(int frame[], int n) {
  int min = frame[0];
  int min_index = 0;
  for (int i = 1; i < n; i++) {
    if (frame[i] < min) {
      min = frame[i];
      min_index = i;
    }
  }
  return min_index;
}

// A function to implement LRU page replacement algorithm
int lru(int pages[], int n, int no_phys_pages) {
  int frame[no_phys_pages]; // An array to store the pages in the frame
  int index[no_phys_pages]; // An array to store the indexes of the pages in the frame
  int page_faults = 0; // A variable to count the number of page faults
  int next_index = 0; // A variable to store the next index to be assigned

  // Initialize the frame and index arrays with -1
  for (int i = 0; i < no_phys_pages; i++) {
    frame[i] = -1;
    index[i] = -1;
  }

  // Traverse the pages array
  for (int i = 0; i < n; i++) {
    int page = pages[i];
    // Check if the page is already in the frame
    int j = 0;
    for (j; j < no_phys_pages; j++) {
      if (frame[j] == page) {
        index[j] = next_index;
        next_index++;
        break;
      }
    }

    if (j == no_phys_pages) { //handel page fault
      int min_index = findMin(index, no_phys_pages);
      frame[min_index] = page;
      index[min_index] = next_index;
      next_index++; 
      page_faults++; 
    }
  }
  return page_faults;
}

//read mem from file
int readTrace(char* filename, int pages[], int n) {
  FILE* fp = fopen(filename, "r"); // Open the file for reading
  if (fp == NULL) {
    return -1;
  }

  int i = 0; 
  int address; 

  while (i < n && fscanf(fp, "%d", &address) == 1) {
    pages[i] = address;
    i++; 
  }
  fclose(fp); 
  return i; 
}

int main(int argc, char* argv[]) {
  //pars args
  int no_phys_pages = atoi(argv[1]);
  int page_size = atoi(argv[2]);
  char* filename = argv[3];
  //alocate memory
  int* pages = (int*)malloc(sizeof(int) * MAX);
  // read memory from file
  int n = readTrace(filename, pages, MAX); 

  if (n == -1) {
    printf("Error: Cannot read trace from file\n");
    return -1;
  }

  //assign the memory addres via conversion
  for (int i = 0; i < n; i++) {
    pages[i] = pages[i] / page_size;
  }

  // Apply the LRU page replacement algorithm
  int page_faults = lru(pages, n, no_phys_pages);

  // Print the input information
    printf("No physical pages = %d, page size = %d\n", no_phys_pages, page_size);
    printf("Reading memory trace from %s...\nRead %d memory references\n", filename, n);
    printf("Result: %d page faults\n", page_faults);

  // Free the allocated memory
  free(pages);
  return 0;
}
