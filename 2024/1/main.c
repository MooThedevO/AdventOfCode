#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void input_parse(FILE* file, int** list1, int** list2, int* len) {
    char line[256];
    *len = 0;

    while (fgets(line, sizeof(line), file)) {
        (*len)++;
    }

    rewind(file);

    *list1 = (int*)malloc(*len * sizeof(int));
    *list2 = (int*)malloc(*len * sizeof(int));

    for (int i = 0; i < *len; i++) {
        if (fscanf(file, "%d %d", &(*list1)[i], &(*list2)[i]) != 2) {
            perror("Error reading input");
            exit(1);
        }
    }

    qsort(*list1, *len, sizeof(int), compare_ints);
    qsort(*list2, *len, sizeof(int), compare_ints);
}

int calculate_difference(int* list1, int* list2, int len) {
    int result = 0;
    for (int i = 0; i < len; i++) {
        result += abs(list1[i] - list2[i]);
    }
    return result;
}

int calculate_similarity(int* list1, int* list2, int len) {
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

int main() {
    FILE* file = fopen("input.txt", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    int* list1 = NULL;
    int* list2 = NULL;
    int len = 0;

    input_parse(file, &list1, &list2, &len);

    printf("\nHello, world!\n\n");
    printf("Here's the solution to Advent of Code Day 1\n\n");
    printf("Total distance is: %d\n", calculate_difference(list1, list2, len));
    printf("Similarity score is: %d\n", calculate_similarity(list1, list2, len));

    free(list1);
    free(list2);

    fclose(file);

    return 0;
}
