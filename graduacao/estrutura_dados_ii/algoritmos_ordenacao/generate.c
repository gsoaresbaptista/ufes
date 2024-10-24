#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    // Check the number of parameters
    if (argc == 1) {
        printf("Error: Please inform the size of the list you want to generate.\n");
        return 1;
    } else if (argc == 2) {
        printf("Error: Please provide the path to the output file.\n");
        return 1;
    }

    // TODO: add checks
    int min_value = -1, max_value = -1;
    if (argc == 5) {
        min_value = atoi(argv[3]);
        max_value = atoi(argv[4]);
    }

    // Check number
    unsigned int vector_size = atoi(argv[1]);
    if (!vector_size) {
        printf("ERROR: Please inform a valid number!\n", vector_size);
    }

    FILE *numbers_file = fopen(argv[2], "w");
    srand(time(NULL));

    for (unsigned int i = 0; i < vector_size; i++) {
        int random_number = (min_value != -1) ? (
            rand() % (max_value - min_value) + min_value
        ) : rand();
        fprintf(numbers_file, "%d ", random_number);
    }

    fclose(numbers_file);

    return 0;
}
