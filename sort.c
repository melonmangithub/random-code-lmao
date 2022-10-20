#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

enum menu_option {
    QUICK_SORT,
    INSERTION_SORT,
    MERGE_SORT,
    QUIT
};

bool menu();
int get_option();

void merge_sort(char** input, int left, int right);
void merge(char** input, int left, int middle, int right);
void quick_sort(char** array, int low, int high);
int partition(char** array, int low, int high);
void putarrs(char** old_array, char** new_array, int len);
char** strscpy(char** array, int len);
void strswap(char** a, char** b);
void freestrs(char** array, int len);


int input_len;
char* input[] = {
    "nickel",
    "iron",
    "silver",
    "tin",
    "lead",
    "cobalt",
    "titanium",
    "platinum",
    "zinc"
};

// entry point
int main(int argc, char** argv) {
    input_len = sizeof(input) / sizeof(input[0]);
    bool state = true;

    do {
        state = menu();
    } while (state);
    
    return 0;
}

// main driver function, returns true if the user is continuing, false if the program needs to exit
bool menu() {
    puts("Choose a algorithm from below or quit to exit");
    puts(
        " - Quick Sort     | Best: O(n log n) | Worst: O(n^2)     | Average: O(n log n)\n"
        " - Insertion Sort | Best: O(n)       | Worst: O(n^2)     | Average: O(n^2)\n"
        " - Merge Sort     | Best: O(n log n) | Worst: O(n log n) | Average: O(n log n)"
    );

    int option = get_option();

    if (option == QUIT) {
        return false;
    } else if (option == -1) {
        printf("Inavlid option\n\n");
        return true;
    }

    time_t start, end;
    char** output = strscpy(input, input_len);

    time(&start);

    switch (option) {
        case MERGE_SORT: merge_sort(output, 0, input_len - 1); break;
        case QUICK_SORT: quick_sort(output, 0, input_len - 1); break;
        case INSERTION_SORT: break;
    }

    time(&end);

    putarrs(input, output, input_len);
    printf("Sorting finished in %s %lld ms\n\n", end - start == 0 ? ">" : "", end - start);

    return true;
}

// retrieves int representing option entered
// cant use strings in switch case
// (i will literally write a compiler specifically to add this feature some day)
// so i convert it to an enum value
int get_option() {
    // prompt
    printf("$ ");

    // get input
    char buf[32] = {0};
    fgets(buf, sizeof(buf), stdin);

    // take off the '\n' char at the end so itll work with strcmp
    buf[strlen(buf) - 1] = '\0';

    // convert string to option
    // excuse the ternary operators, perfer it to ladders of if statements
    return (
        !strcasecmp(buf, "Quick Sort") ? QUICK_SORT :
        !strcasecmp(buf, "Insertion Sort") ? INSERTION_SORT :
        !strcasecmp(buf, "Merge Sort") ? MERGE_SORT :
        !strcasecmp(buf, "Quit") ? QUIT : -1
    );
}

// literally just prints an array
void putarrs(char** old_array, char** new_array, int len) {
    printf("\n   %-8s   %-8s\n", "Unsorted", "Sorted");

    for (int i = 0; i < len; i++) {
        printf("%d) %-8s | %-8s\n", i, old_array[i], new_array[i]);
    }
}

// swaps two strings around
void strswap(char** a, char** b) {
    char* tmp = *a;
    *a = *b;
    *b = tmp;
}

// copies a string array
char** strscpy(char** array, int len) {
    char** out = malloc(sizeof(char*) * len);

    for (int i = 0; i < len; i++) {
        out[i] = strdup(array[i]);
    }

    return out;
}

// frees a string array
void freestrs(char** array, int len) {
    for (int i = 0; i < len; i++) {
        free(array[i]);
    }

    free(array);
}

// main function for merge sorting
void merge_sort(char** array, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        // sort halves
        merge_sort(array, left, middle);
        merge_sort(array, middle + 1, right);

        // slap em together
        merge(array, left, middle, right);
    }
}

void merge(char** array, int left, int middle, int right) {
    // get the size of the two tmp arrays
    int left_size = middle - left + 1;
    int right_size = right - middle;

    // setup left tmp array
    char* left_tmp[left_size];

    // initalize it
    for (int i = 0; i < left_size; i++) {
        left_tmp[i] = array[left + i];
    }

    // setup right array
    char* right_tmp[right_size];

    // initalize it
    for (int i = 0; i < right_size; i++) {
        right_tmp[i] = array[middle + i + 1];
    }

    // various indecies needed
    int li = 0;
    int ri = 0;
    int i = left;

    while (li < left_size && ri < right_size) {
        if (strcasecmp(left_tmp[li], right_tmp[ri]) <= 0) {
            array[i] = left_tmp[li];
            li++;
        } else {
            array[i] = right_tmp[ri];
            ri++;
        }

        i++;
    }

    // copy the rest of left
    while (li < left_size) {
        array[i++] = left_tmp[li++];
    }

    // and copy the rest of right
    while (ri < right_size) {
        array[i++] = right_tmp[ri++];
    }
}

int partition(char** array, int low, int high) {
    char* pivot = array[high];
    int i = (low - 1);

    for (int x = low; x <= high - 1; x++) {
        if (strcasecmp(array[x], pivot) < 0) {
            i++;
            strswap(&array[i], &array[x]);
        }
    }

    strswap(&array[i + 1], &array[high]);
    return (i + 1);
}

void quick_sort(char** array, int low, int high) {
    if (low < high) {
        int pivot = partition(array, low, high);

        quick_sort(array, low, pivot - 1);
        quick_sort(array, pivot + 1, high);
    }
}