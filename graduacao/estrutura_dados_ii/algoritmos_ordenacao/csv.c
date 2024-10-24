#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern void selection_sort(unsigned int *array, unsigned int length);
extern void bubble_sort(unsigned int *array, unsigned int length);
extern void insertion_sort(unsigned int *array, unsigned int length);
extern void merge_sort(unsigned int *array, unsigned int length);
extern void quick_sort(unsigned int *array, unsigned int length);

typedef void (*sort_func)(unsigned int *, unsigned int);

unsigned int* read_vector(const char *path, unsigned int length);
void print_vector(unsigned int *vector, int length);
int sanity_check(unsigned int *vector, int length);

int main() {
    // Auxiliary vectors to read the files
    int lengths[] = {
        10, 100, 1000, 10000, 100000, 1000000
    };
    char numbers_qtd[][9] = {
        "10.txt\0",
        "100.txt\0",
        "1k.txt\0",
        "10k.txt\0",
        "100k.txt\0",
        "1M.txt\0",
    };

    // Auxiliary vectors to get sort function
   sort_func algorithms[4] = {
        selection_sort,
        bubble_sort,
        insertion_sort,
        merge_sort,
    };
    char algorithms_names[4][100] = {
        "selection_sort\0",
        "bubble_sort\0",
        "insertion_sort\0",
        "merge_sort\0",
    };

    // Open csv file
    FILE *csv_file = fopen("algorithms.csv", "r");

    if (!csv_file) {
        csv_file = fopen("algorithms.csv", "w");
        fprintf(csv_file, "algorithm,numbers,seconds\n");
    } else {
        csv_file = fopen("algorithms.csv", "a");
    }

    for (int algorithm_id = 0; algorithm_id < 1; algorithm_id++) {
        sort_func sort_fn = algorithms[algorithm_id];

        for (int i = 0; i < 6; i++) {
            // Read numbers file
            char path[20] = "files/";
            strcat(path, numbers_qtd[i]);
            unsigned int *numbers = read_vector(path, lengths[i]);

            // Run the sort algorithm
            clock_t start = clock();
            sort_fn(numbers, lengths[i]);
            clock_t end = clock();
            double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            fprintf(csv_file, "%s,%d,%lf\n",
                algorithms_names[algorithm_id], lengths[i], time_spent);
            fflush(csv_file);
        }
    }

    fclose(csv_file);

    return 0;
}

unsigned int* read_vector(const char *path, unsigned int size) {
    FILE *numbers_file = fopen(path, "r");
    unsigned int *numbers = malloc(sizeof(unsigned int) * size);
    for (unsigned int i = 0; i < size ; i++)
        fscanf(numbers_file, "%d", &numbers[i]);
    fclose(numbers_file);
    return numbers;
}

void print_vector(unsigned int *vector, int length) {
    for (int i = 0; i < length; i++) {
        printf("%d ", vector[i]);
    }
    printf("\n");
}

int sanity_check(unsigned int *vector, int length) {
    int sorted = 1;

    for (int i = 1; i < length; i++) {
        if (vector[i - 1] > vector[i]) {
            sorted = 0;
            break;
        }
    }

    return sorted;
}
