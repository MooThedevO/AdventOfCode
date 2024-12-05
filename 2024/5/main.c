#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

typedef struct {
    int x, y;
} Order;

typedef struct {
    int* test;
    int length;
} Test;

void read_input(const char* filename, char*** input, int* num_lines);
void input_parse(char** input, int num_lines, Order** orders, int* num_orders, Test** tests, int* num_tests);
void order_pages(Order* orders, int num_orders, int** ordered_pages, int max_page);
int order_tests(Test* tests, int num_tests, int** ordered_pages, int max_page, int* failed_count, Test** failed_tests);
void order_failed(Test* failed_tests, int failed_count, int** ordered_pages, int max_page, Test** ordered_failed_tests, int* ordered_failed_count);
void free_input(char** input, int num_lines);
void free_tests(Test* tests, int num_tests);

int main() {
    printf("\nHello, world!\n\n");
    printf("Here's the solution to Advent of Code Day 4\n\n");

    char** input = NULL;
    int num_lines = 0;
    read_input("input.txt", &input, &num_lines);

    Order* orders = NULL;
    int num_orders = 0;
    Test* tests = NULL;
    int num_tests = 0;
    input_parse(input, num_lines, &orders, &num_orders, &tests, &num_tests);

    int max_page = 0;
    for (int i = 0; i < num_orders; i++) {
        if (orders[i].x > max_page) max_page = orders[i].x;
        if (orders[i].y > max_page) max_page = orders[i].y;
    }

    int* ordered_pages = (int*)calloc((max_page + 1) * (max_page + 1), sizeof(int));

    order_pages(orders, num_orders, &ordered_pages, max_page);

    int failed_count = 0;
    Test* failed_tests = NULL;
    int ordered_sum = order_tests(tests, num_tests, &ordered_pages, max_page, &failed_count, &failed_tests);
    printf("Middle page number from correctly-ordered updates: %d\n", ordered_sum);

    Test* ordered_failed_tests = NULL;
    int ordered_failed_count = 0;
    order_failed(failed_tests, failed_count, &ordered_pages, max_page, &ordered_failed_tests, &ordered_failed_count);
    int failed_ordered_sum = order_tests(ordered_failed_tests, ordered_failed_count, &ordered_pages, max_page, &failed_count, &failed_tests);
    printf("Middle page number from failed-ordered updates: %d\n", failed_ordered_sum);

    free_input(input, num_lines);
    free(orders);
    free(tests);
    free(failed_tests);
    free(ordered_failed_tests);
    free(ordered_pages);

    return 0;
}

void read_input(const char* filename, char*** input, int* num_lines) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    *input = (char**)malloc(MAX_LINE_LENGTH * sizeof(char*));
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        (*input)[*num_lines] = (char*)malloc((strlen(line) + 1) * sizeof(char));
        strcpy((*input)[*num_lines], line);
        (*num_lines)++;
        *input = (char**)realloc(*input, (*num_lines + 1) * sizeof(char*));
    }

    fclose(file);
}

void input_parse(char** input, int num_lines, Order** orders, int* num_orders, Test** tests, int* num_tests) {
    for (int i = 0; i < num_lines; i++) {
        if (strchr(input[i], '|')) {
            int x, y;
            sscanf(input[i], "%d|%d", &x, &y);
            *orders = (Order*)realloc(*orders, (*num_orders + 1) * sizeof(Order));
            (*orders)[*num_orders].x = x;
            (*orders)[*num_orders].y = y;
            (*num_orders)++;
        } else if (strchr(input[i], ',')) {
            int* test = NULL;
            int len = 0;
            char* token = strtok(input[i], ",");
            while (token) {
                test = (int*)realloc(test, (len + 1) * sizeof(int));
                test[len++] = atoi(token);
                token = strtok(NULL, ",");
            }
            *tests = (Test*)realloc(*tests, (*num_tests + 1) * sizeof(Test));
            (*tests)[*num_tests].test = test;
            (*tests)[*num_tests].length = len;
            (*num_tests)++;
        }
    }
}

void order_pages(Order* orders, int num_orders, int** ordered_pages, int max_page) {
    for (int i = 0; i < num_orders; i++) {
        int x = orders[i].x;
        int y = orders[i].y;
        (*ordered_pages)[x * (max_page + 1) + y] = 1;
    }
}

int order_tests(Test* tests, int num_tests, int** ordered_pages, int max_page, int* failed_count, Test** failed_tests) {
    int result = 0;
    for (int i = 0; i < num_tests; i++) {
        int* test = tests[i].test;
        int len = tests[i].length;
        int test_passed = 1;

        for (int j = 0; j < len - 1; j++) {
            if (!(*ordered_pages)[test[j] * (max_page + 1) + test[j + 1]]) {
                test_passed = 0;
                *failed_tests = (Test*)realloc(*failed_tests, (*failed_count + 1) * sizeof(Test));
                (*failed_tests)[*failed_count] = tests[i];
                (*failed_count)++;
                break;
            }
        }

        if (test_passed) {
            result += test[len / 2];
        }
    }
    return result;
}

void order_failed(Test* failed_tests, int failed_count, int** ordered_pages, int max_page, Test** ordered_failed_tests, int* ordered_failed_count) {
    for (int i = 0; i < failed_count; i++) {
        int* test = failed_tests[i].test;
        int len = failed_tests[i].length;

        int* temp_test = (int*)malloc(len * sizeof(int));
        memcpy(temp_test, test, len * sizeof(int));

        int sorted = 0;
        while (!sorted) {
            sorted = 1;
            for (int j = 0; j < len - 1; j++) {
                if (!(*ordered_pages)[temp_test[j] * (max_page + 1) + temp_test[j + 1]]) {
                    int temp = temp_test[j];
                    temp_test[j] = temp_test[j + 1];
                    temp_test[j + 1] = temp;
                    sorted = 0;
                }
            }
        }

        *ordered_failed_tests = (Test*)realloc(*ordered_failed_tests, (*ordered_failed_count + 1) * sizeof(Test));
        (*ordered_failed_tests)[*ordered_failed_count].test = temp_test;
        (*ordered_failed_tests)[*ordered_failed_count].length = len;
        (*ordered_failed_count)++;
    }
}

void free_input(char** input, int num_lines) {
    for (int i = 0; i < num_lines; i++) {
        free(input[i]);
    }
    free(input);
}

void free_tests(Test* tests, int num_tests) {
    for (int i = 0; i < num_tests; i++) {
        free(tests[i].test);
    }
    free(tests);
}
