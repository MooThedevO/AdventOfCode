#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_ROWS 1000
#define MAX_COLS 1000

int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

char** read_input(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return NULL;
    }

    char** grid = malloc(MAX_ROWS * sizeof(char*));
    char line[MAX_COLS];
    *rows = 0;

    while (fgets(line, sizeof(line), file)) {
        grid[*rows] = malloc(strlen(line) + 1);
        strcpy(grid[*rows], line);
        (*cols) = strlen(line) - 1;
        (*rows)++;
    }
    
    fclose(file);
    return grid;
}

void find_starting_pos(char** grid, int rows, int cols, int* start_x, int* start_y) {
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if (grid[x][y] == '^') {
                *start_x = x;
                *start_y = y;
                return;
            }
        }
    }
}

int move(char** grid, int rows, int cols, int start_x, int start_y) {
    int steps = 0;
    int current_direction = 0;  // Start moving up
    int current_x = start_x, current_y = start_y;

    while (true) {
        grid[current_x][current_y] = 'X';
        int new_x = current_x + directions[current_direction][0];
        int new_y = current_y + directions[current_direction][1];

        if (new_x < 0 || new_x >= rows || new_y < 0 || new_y >= cols) {
            steps++;
            break;
        }

        if (grid[new_x][new_y] == '#') {
            current_direction = (current_direction + 1) % 4;
            continue;
        }

        current_x = new_x;
        current_y = new_y;

        if (grid[current_x][current_y] != 'X') {
            steps++;
        }
    }

    return steps;
}

int will_loop(char** grid, int rows, int cols, int start_x, int start_y) {
    int loops = 0;
    int current_direction = 0;  // Start moving up
    int current_x = start_x, current_y = start_y;

    bool*** visited = malloc(rows * sizeof(bool**));
    for (int i = 0; i < rows; i++) {
        visited[i] = malloc(cols * sizeof(bool*));
        for (int j = 0; j < cols; j++) {
            visited[i][j] = malloc(4 * sizeof(bool));  
            memset(visited[i][j], 0, 4 * sizeof(bool));
        }
    }

    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if (grid[x][y] == 'X') { 
                grid[x][y] = '#'; 

                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        memset(visited[i][j], 0, 4 * sizeof(bool));
                    }
                }

                bool loop_detected = false;
                int position_x = current_x;
                int position_y = current_y;
                int direction = current_direction;

                while (true) {
                    if (visited[position_x][position_y][direction]) {
                        loop_detected = true;
                        break;
                    }
                    visited[position_x][position_y][direction] = true;

                    int new_x = position_x + directions[direction][0];
                    int new_y = position_y + directions[direction][1];

                    if (new_x < 0 || new_x >= rows || new_y < 0 || new_y >= cols) {
                        break;  
                    }

                    if (grid[new_x][new_y] == '#') {
                        direction = (direction + 1) % 4;
                        continue;
                    }

                    position_x = new_x;
                    position_y = new_y;
                }

                if (loop_detected) {
                    loops++;
                }

                grid[x][y] = 'X';  
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            free(visited[i][j]);
        }
        free(visited[i]);
    }
    free(visited);

    return loops;
}

int main() {
    clock_t start_time = clock();

    int rows = 0, cols = 0;
    char** grid = read_input("input.txt", &rows, &cols);

    int start_x, start_y;
    find_starting_pos(grid, rows, cols, &start_x, &start_y);

    printf("Distinct positions the guard will visit before leaving the mapped area: %d\n", move(grid, rows, cols, start_x, start_y));
    printf("Different positions to cause a loop: %d\n", will_loop(grid, rows, cols, start_x, start_y));

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Elapsed time: %f seconds\n", elapsed_time);

    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);

    return 0;
}
