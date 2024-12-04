#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD "XMAS"
#define WORD_LEN 4

int count_all(char **input, int rows, int cols);
int is_xmas_at_position(int x, int y, int dx, int dy, char **input, int rows, int cols);
int count_xmas_x_shapes(char **input, int rows, int cols);
int check_x_shape(int x, int y, char **input);
void read_input(char filename[], char ***input, int *rows, int *cols);

int main() {
    char **input;
    int rows, cols;

    printf("\nHello, world!\n");
    printf("Here's the solution to Advent of Code Day 4\n");

    read_input("input.txt", &input, &rows, &cols);

    if (input != NULL) {
        printf("Occurrences of XMAS: %d\n", count_all(input, rows, cols));
        printf("Occurrences of XMAS in X shape: %d\n", count_xmas_x_shapes(input, rows, cols));

        // Free allocated memory
        for (int i = 0; i < rows; i++) {
            free(input[i]);
        }
        free(input);
    } else {
        printf("Failed to read input correctly.\n");
    }

    return 0;
}

void read_input(char filename[], char ***input, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return;
    }

    *rows = 0;
    *cols = 0;
    char buffer[1024]; 

    *input = malloc(sizeof(char*));

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        (*rows)++;
        
        *input = realloc(*input, (*rows) * sizeof(char*));

        (*input)[*rows - 1] = malloc(strlen(buffer) + 1);
        strcpy((*input)[*rows - 1], buffer);

        if (*rows == 1) {
            *cols = strlen(buffer);
        }
    }

    fclose(file);
}

int count_all(char **input, int rows, int cols) {
    int count = 0;
    int directions[8][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, 
        {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int d = 0; d < 8; d++) {
                int dx = directions[d][0], dy = directions[d][1];
                if (is_xmas_at_position(i, j, dx, dy, input, rows, cols)) {
                    count++;
                }
            }
        }
    }

    return count;
}

int is_xmas_at_position(int x, int y, int dx, int dy, char **input, int rows, int cols) {
    for (int i = 0; i < WORD_LEN; i++) {
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx < 0 || nx >= rows || ny < 0 || ny >= cols || input[nx][ny] != WORD[i]) {
            return 0;
        }
    }
    return 1;
}

int count_xmas_x_shapes(char **input, int rows, int cols) {
    int count = 0;
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (input[i][j] == 'A' && check_x_shape(i, j, input)) {
                count++;
            }
        }
    }
    return count;
}

int check_x_shape(int x, int y, char **input) {
    char positive_direction[3] = {input[x-1][y-1], input[x+1][y+1], '\0'};
    char negative_direction[3] = {input[x-1][y+1], input[x+1][y-1], '\0'};

    return (strstr(positive_direction, "MS") || strstr(positive_direction, "SM")) &&
           (strstr(negative_direction, "MS") || strstr(negative_direction, "SM"));
}
