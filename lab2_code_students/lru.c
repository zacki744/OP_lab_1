// A C program to implement LRU page replacement algorithm
#include <stdio.h>
#include <stdlib.h>

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
int lru(int pages[], int n, int capacity) {
  int frame[capacity]; // An array to store the pages in the frame
  int index[capacity]; // An array to store the indexes of the pages in the frame
  int page_faults = 0; // A variable to count the number of page faults
  int next_index = 0; // A variable to store the next index to be assigned

  // Initialize the frame and index arrays with -1
  for (int i = 0; i < capacity; i++) {
    frame[i] = -1;
    index[i] = -1;
  }

  // Traverse the pages array
  for (int i = 0; i < n; i++) {
    int page = pages[i]; // The current page
    int found = 0; // A flag to indicate whether the page is found in the frame or not

    // Check if the page is already in the frame
    for (int j = 0; j < capacity; j++) {
      if (frame[j] == page) {
        found = 1;
        index[j] = next_index; // Update the index of the page
        next_index++; // Increment the next index
        break;
      }
    }

    // If the page is not found in the frame
    if (found == 0) {
      // Find the page with the minimum index in the frame
      int min_index = findMin(index, capacity);
      // Replace that page with the current page
      frame[min_index] = page;
      index[min_index] = next_index; // Update the index of the page
      next_index++; // Increment the next index
      page_faults++; // Increment the number of page faults
    }
  }

  // Return the number of page faults
  return page_faults;
}

// A function to read the memory trace from a file
int readTrace(char* filename, int pages[], int n) {
  FILE* fp = fopen(filename, "r"); // Open the file for reading
  if (fp == NULL) {
    printf("Error: Cannot open file %s\n", filename);
    return -1;
  }
  int i = 0; // A variable to store the index of the pages array
  int address; // A variable to store the memory address
  // Read the file line by line until the end of file or the pages array is full
  while (i < n && fscanf(fp, "%d", &address) == 1) {
    pages[i] = address; // Store the address in the pages array
    i++; // Increment the index of the pages array
  }
  fclose(fp); // Close the file
  return i; // Return the number of pages read
}

// The main function
int main(int argc, char* argv[]) {
  // Check the number of arguments
  if (argc != 4) {
    printf("Usage: ./lru no_phys_pages page_size filename\n");
    return -1;
  }

  // Parse the arguments
  int capacity = atoi(argv[1]); // The number of physical pages
  int page_size = atoi(argv[2]); // The page size
  char* filename = argv[3]; // The name of the trace file

  // Print the input information
  printf("No physical pages = %d, page size = %d\n", capacity, page_size);
  printf("Reading memory trace from %s...\n", filename);

  // Define the maximum number of pages to be read
  int max_pages = 100000;

  // Allocate an array to store the pages
  int* pages = (int*)malloc(sizeof(int) * max_pages);
  if (pages == NULL) {
    printf("Error: Cannot allocate memory for pages\n");
    return -1;
  }

  // Read the memory trace from the file
  int n = readTrace(filename, pages, max_pages);
  if (n == -1) {
    printf("Error: Cannot read trace from file\n");
    return -1;
  }
  printf("Read %d memory references\n", n);

  // Convert the memory addresses to page numbers
  for (int i = 0; i < n; i++) {
    pages[i] = pages[i] / page_size;
  }

  // Apply the LRU page replacement algorithm
  int page_faults = lru(pages, n, capacity);

  // Print the result
  printf("Result: %d page faults\n", page_faults);

  // Free the allocated memory
  free(pages);

  return 0;
}
