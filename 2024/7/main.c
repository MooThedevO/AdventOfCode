#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CHUNK_SIZE 100

int test_numbers(long long int result, long long int *numbers, char operators[], int num_numbers, long long int target, int *test_success);
long long int apply_operator(long long int n1, long long int n2, char operator);
void free_memory(char **lines, int line_count, long long int *tests, long long int **numbers, int num_tests);
void input_parse(char **lines, int line_count, long long int **tests, long long int ***numbers, int *num_tests, int **num_numbers);
int read_input(const char *filename, char ***lines);
long long int test_chunk(long long int *tests, long long int **numbers, int *num_numbers, int chunk_size, char operators[]);

int read_input(const char *filename, char ***lines) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    size_t line_size = 1024;
    char *line = malloc(line_size);
    int line_count = 0;

    *lines = malloc(sizeof(char *) * 500);
    while (fgets(line, line_size, file)) {
        if (line_count >= 500) {
            *lines = realloc(*lines, sizeof(char *) * (line_count + 500));
        }
        (*lines)[line_count] = strdup(line);
        line_count++;
    }

    free(line);
    fclose(file);
    return line_count;
}

void input_parse(char **lines, int line_count, long long int **tests, long long int ***numbers, int *num_tests, int **num_numbers) {
    *tests = malloc(sizeof(long long int) * line_count);
    *numbers = malloc(sizeof(long long int *) * line_count);
    *num_numbers = malloc(sizeof(int) * line_count);

    for (int i = 0; i < line_count; i++) {
        char *line = lines[i];

        char *token = strtok(line, ":");
        (*tests)[i] = atoll(token);

        token = strtok(NULL, ":");

        if (token != NULL) {
            long long int *nums = malloc(sizeof(long long int) * 10);
            int num_count = 0;

            token = strtok(token, " ");
            while (token != NULL) {
                if (num_count >= 10) {
                    nums = realloc(nums, sizeof(long long int) * (num_count + 10));
                }
                nums[num_count++] = atoll(token);
                token = strtok(NULL, " ");
            }

            (*numbers)[i] = nums;
            (*num_numbers)[i] = num_count;
        } else {
            (*numbers)[i] = NULL;
            (*num_numbers)[i] = 0;
        }
    }

    *num_tests = line_count;
}

long long int apply_operator(long long int n1, long long int n2, char operator) {
    long long int result = 0;
    switch (operator) {
        case '+':
            result = n1 + n2;
            break;
        case '*':
            result = n1 * n2;
            break;
        case '|': {
            char str1[40], str2[40];
            snprintf(str1, sizeof(str1), "%lld", n1);
            snprintf(str2, sizeof(str2), "%lld", n2);
            strcat(str1, str2);
            result = atoll(str1);
            break;
        }
        default:
            result = 0;
            break;
    }
    return result;
}

long long int test_chunk(long long int *tests, long long int **numbers, int *num_numbers, int chunk_size, char operators[]) {
    long long int result = 0;

    for (int i = 0; i < chunk_size; i++) {
        int test_success = 0;
        long long int initial_result = numbers[i][0];

        if (test_numbers(initial_result, numbers[i] + 1, operators, num_numbers[i] - 1, tests[i], &test_success)) {
            if (test_success) {
                result += tests[i];
            }
        }
    }

    return result;
}

int test_numbers(long long int result, long long int *numbers, char operators[], int num_numbers, long long int target, int *test_success) {
    if (num_numbers == 0) {
        return result == target;
    }

    for (int i = 0; operators[i] != '\0'; i++) {
        long long int new_result = apply_operator(result, numbers[0], operators[i]);

        if (new_result > target) {
            continue;
        }

        if (new_result == target) {
            *test_success = 1;
            return 1;
        }

        if (test_numbers(new_result, &numbers[1], operators, num_numbers - 1, target, test_success)) {
            return 1;
        }
    }

    return 0;
}

void free_memory(char **lines, int line_count, long long int *tests, long long int **numbers, int num_tests) {
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
    free(tests);

    for (int i = 0; i < num_tests; i++) {
        free(numbers[i]);
    }
    free(numbers);
}

int main() {
    clock_t start_time = clock();

    char **lines;
    int line_count = read_input("input.txt", &lines);
    if (line_count == -1) {
        return 1;
    }

    long long int *tests;
    long long int **numbers;
    int *num_numbers = malloc(sizeof(int) * line_count);
    int num_tests = 0;
    input_parse(lines, line_count, &tests, &numbers, &num_tests, &num_numbers);

    long long int total_result_part1 = 0;
    long long int total_result_part2 = 0;
    char part1_operators[] = {'+', '*', '\0'};
    char part2_operators[] = {'+', '*', '|', '\0'};

    for (int chunk_start = 0; chunk_start < num_tests; chunk_start += CHUNK_SIZE) {
        int chunk_size = (chunk_start + CHUNK_SIZE <= num_tests) ? CHUNK_SIZE : num_tests - chunk_start;

        long long int result_part1 = test_chunk(tests + chunk_start, numbers + chunk_start, num_numbers + chunk_start, chunk_size, part1_operators);
        long long int result_part2 = test_chunk(tests + chunk_start, numbers + chunk_start, num_numbers + chunk_start, chunk_size, part2_operators);

        total_result_part1 += result_part1;
        total_result_part2 += result_part2;
    }

    printf("Part 1 Result: %lld\n", total_result_part1);
    printf("Part 2 Result: %lld\n", total_result_part2);

    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %f seconds\n", execution_time);

    free_memory(lines, line_count, tests, numbers, num_tests);
    free(num_numbers);

    return 0;
}
