#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INITIAL_CAPACITY 16

int *list1 = NULL, *list2 = NULL, len = 0;

int compare_ints(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void input_parse(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int capacity = INITIAL_CAPACITY;
    list1 = (int *)malloc(capacity * sizeof(int));
    list2 = (int *)malloc(capacity * sizeof(int));

    if (!list1 || !list2) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%d %d", &list1[len], &list2[len]) == 2) {
        len++;

        if (len >= capacity) {
            capacity *= 2;
            list1 = (int *)realloc(list1, capacity * sizeof(int));
            list2 = (int *)realloc(list2, capacity * sizeof(int));

            if (!list1 || !list2) {
                perror("Memory reallocation failed");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (len == 0) {
        fprintf(stderr, "Error: No valid data found in file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);

    qsort(list1, len, sizeof(int), compare_ints);
    qsort(list2, len, sizeof(int), compare_ints);
}

int calculate_difference(void) {
    int result = 0;
    for (int i = 0; i < len; i++) {
        result += abs(list1[i] - list2[i]);
    }
    return result;
}

int calculate_similarity(void) {
    int result = 0;
    for (int i = 0; i < len; i++) {
        int count = 0;
        for (int j = 0; j < len; j++) {
            if (list2[j] == list1[i]) {
                count++;
            }
        }
        result += count * list1[i];
    }
    return result;
}

int main(void) {
    input_parse("input.txt");

    printf("\nSolution to Advent of Code Day 1\n\n");
    printf("Total distance is: %d\n", calculate_difference());
    printf("Similarity score is: %d\n", calculate_similarity());

    free(list1);
    free(list2);

    return EXIT_SUCCESS;
}
