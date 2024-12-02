#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define INITIAL_CAPACITY 16
#define MAX_REPORT_LENGTH 100
#define LINE_LENGTH 256

int **reports = NULL;
int *report_lengths = NULL;
int report_count = 0;

void read_input(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int capacity = INITIAL_CAPACITY;
    reports = (int **)malloc(capacity * sizeof(int *));
    report_lengths = (int *)malloc(capacity * sizeof(int));

    if (!reports || !report_lengths) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    char line[LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n') continue;

        int *report = (int *)malloc(MAX_REPORT_LENGTH * sizeof(int));
        if (!report) {
            perror("Memory allocation failed");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        int i = 0;
        char *token = strtok(line, " ");  
        
        while (token != NULL) {
            report[i++] = atoi(token);  
            token = strtok(NULL, " ");
        }

        report_lengths[report_count] = i;
        reports[report_count] = report;
        report_count++;

        // Check if we need to increase capacity
        if (report_count >= capacity) {
            capacity *= 2;
            reports = (int **)realloc(reports, capacity * sizeof(int *));
            report_lengths = (int *)realloc(report_lengths, capacity * sizeof(int));

            if (!reports || !report_lengths) {
                perror("Memory reallocation failed");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);

    if (report_count == 0) {
        printf("No reports were found in the input file.\n");
        exit(EXIT_FAILURE);
    }
}

bool is_safe_report(int *report, int length) {
    bool is_increasing = true, is_decreasing = true;

    for (int i = 1; i < length; i++) {
        int diff = abs(report[i] - report[i - 1]);

        if (diff < 1 || diff > 3) {
            return false;
        }

        if (report[i] <= report[i - 1]) {
            is_increasing = false;
        }

        if (report[i] >= report[i - 1]) {
            is_decreasing = false;
        }
    }

    return is_increasing || is_decreasing;
}

int is_safe_report_count() {
    int safe_count = 0;

    for (int i = 0; i < report_count; i++) {
        if (is_safe_report(reports[i], report_lengths[i])) {
            safe_count++;
        }
    }

    return safe_count;
}

int is_safe_with_removal_count() {
    int safe_count = 0;

    for (int i = 0; i < report_count; i++) {
        for (int j = 0; j < report_lengths[i]; j++) {
            int new_report[MAX_REPORT_LENGTH];
            int new_length = 0;

            for (int k = 0; k < report_lengths[i]; k++) {
                if (k != j) {
                    new_report[new_length++] = reports[i][k];
                }
            }

            if (is_safe_report(new_report, new_length)) {
                safe_count++;
                break;  
            }
        }
    }

    return safe_count;
}

void free_memory() {
    for (int i = 0; i < report_count; i++) {
        free(reports[i]);
    }
    free(reports);
    free(report_lengths);
}

int main() {
    read_input("input.txt");

    printf("\nHello, Advent of code!\n\n");
    printf("Solution to the day 2 problem\n\n");
    printf("Number of safe reports (without removal): %d\n", is_safe_report_count());
    printf("Number of safe reports (with removal): %d\n", is_safe_with_removal_count());

    free_memory();

    return EXIT_SUCCESS;
}
