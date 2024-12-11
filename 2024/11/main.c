#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_input(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char *)malloc(size + 1);
    if (!data) {
        perror("Memory allocation for data failed");
        exit(1);
    }

    fread(data, 1, size, file);
    data[size] = '\0';

    fclose(file);
    return data;
}

char** parse_input(char *input, int *stone_count) {
    *stone_count = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            (*stone_count)++;
        }
    }
    (*stone_count)++;

    char **stones = (char **)malloc((*stone_count) * sizeof(char *));
    if (!stones) {
        perror("Memory allocation for stones failed");
        exit(1);
    }

    char *stone = strtok(input, " ");
    int index = 0;
    while (stone != NULL) {
        stones[index++] = stone;
        stone = strtok(NULL, " ");
    }

    return stones;
}

typedef struct {
    char* stone;
    long long int count;
} StoneCount;

void add_to_count(StoneCount **counts, int *count_size, const char *stone, long long int count) {
    for (int i = 0; i < *count_size; i++) {
        if (strcmp((*counts)[i].stone, stone) == 0) {
            (*counts)[i].count += count;
            return;
        }
    }

    *counts = (StoneCount *)realloc(*counts, (*count_size + 1) * sizeof(StoneCount));
    (*counts)[*count_size].stone = strdup(stone);
    (*counts)[*count_size].count = count;
    (*count_size)++;
}

void blink(StoneCount **counts, int *count_size) {
    StoneCount *new_counts = (StoneCount *)malloc(*count_size * sizeof(StoneCount));
    int new_count_size = 0;

    for (int i = 0; i < *count_size; i++) {
        const char *stone = (*counts)[i].stone;
        long long int count = (*counts)[i].count;

        if (strlen(stone) % 2 == 0) {
            int half_len = strlen(stone) / 2;
            char left_half[half_len + 1];
            strncpy(left_half, stone, half_len);
            left_half[half_len] = '\0';

            char right_half[strlen(stone) - half_len + 1];
            strcpy(right_half, stone + half_len);

            add_to_count(&new_counts, &new_count_size, left_half, count);
            long long int right_value = (strcmp(right_half, "0") == 0) ? 0 : atoll(right_half);
            char right_str[21];
            snprintf(right_str, 21, "%lld", right_value);
            add_to_count(&new_counts, &new_count_size, right_str, count);
        } else {
            long long int new_value = (atoll(stone) == 0) ? 1 : atoll(stone) * 2024;
            char new_stone[21];
            snprintf(new_stone, 21, "%lld", new_value);

            add_to_count(&new_counts, &new_count_size, new_stone, count);
        }
    }

    for (int i = 0; i < *count_size; i++) {
        free((*counts)[i].stone);
    }
    free(*counts);
    *counts = new_counts;
    *count_size = new_count_size;
}

long long int calc_blinks(StoneCount *counts, int count_size, int times) {
    for (int i = 0; i < times; i++) {
        blink(&counts, &count_size);
    }

    long long int total_count = 0;
    for (int i = 0; i < count_size; i++) {
        total_count += counts[i].count;
    }

    return total_count;
}

int main() {
    printf("\nHello, world!\n");
    printf("Here's the solution to Advent of Code Day 1\n");

    char *input = read_input("input.txt");

    int stone_count = 0;
    char **stones = parse_input(input, &stone_count);

    StoneCount *counts = (StoneCount *)malloc(stone_count * sizeof(StoneCount));
    int count_size = 0;
    for (int i = 0; i < stone_count; i++) {
        add_to_count(&counts, &count_size, stones[i], 1);
    }

    printf("Part 1, 25 blinks : %lld\n", calc_blinks(counts, count_size, 25));
    printf("Part 2, 75 blinks : %lld\n", calc_blinks(counts, count_size, 75));

    free(input);
    free(stones);
    for (int i = 0; i < count_size; i++) {
        free(counts[i].stone);
    }
    free(counts);

    return 0;
}
