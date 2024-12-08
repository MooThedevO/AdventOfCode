#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x, y;
} Position;

typedef struct Antenna {
    char frequency;
    Position *positions;
    int count;
    struct Antenna *next;
} Antenna;

typedef struct {
    int x, y;
} Antinode;

int is_unique(Antinode *antinodes, int *size, int x, int y) {
    for (int i = 0; i < *size; i++) {
        if (antinodes[i].x == x && antinodes[i].y == y) {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}

void add_antinode(Antinode *antinodes, int *size, int x, int y) {
    if (is_unique(antinodes, size, x, y)) {
        antinodes[*size].x = x;
        antinodes[*size].y = y;
        (*size)++;
    }
}

void read_input(const char *filename, char ***grid, int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file!\n");
        exit(1);
    }

    char buffer[1024];
    *rows = 0;

    *grid = malloc(sizeof(char*) * 1);

    while (fgets(buffer, sizeof(buffer), file)) {
        // Resize grid to accommodate more rows
        *grid = realloc(*grid, sizeof(char*) * (*rows + 1));
        (*grid)[*rows] = malloc(strlen(buffer) + 1);
        strcpy((*grid)[*rows], buffer);
        (*rows)++;
    }

    *cols = strlen((*grid)[0]) - 1;  
    fclose(file);
}

void add_antenna(Antenna **head, char freq, Position *pos) {
    Antenna *new_antenna = malloc(sizeof(Antenna));
    new_antenna->frequency = freq;
    new_antenna->positions = malloc(sizeof(Position) * 1);
    new_antenna->positions[0] = *pos;
    new_antenna->count = 1;
    new_antenna->next = *head;
    *head = new_antenna;
}

void get_antennas(char **grid, int rows, int cols, Antenna **head) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char c = grid[i][j];
            if (c == '.') continue;

            Position pos = {i, j};

            Antenna *current = *head;
            int found = 0;
            while (current) {
                if (current->frequency == c) {
                    current->positions = realloc(current->positions, sizeof(Position) * (current->count + 1));
                    current->positions[current->count] = pos;
                    current->count++;
                    found = 1;
                    break;
                }
                current = current->next;
            }

            if (!found) {
                add_antenna(head, c, &pos);
            }
        }
    }
}

void get_antinodes(Antenna *head, int rows, int cols, Antinode *antinodes, int *size) {
    for (Antenna *antenna = head; antenna != NULL; antenna = antenna->next) {
        if (antenna->count < 2) continue; 
        for (int i = 0; i < antenna->count; i++) {
            for (int j = i + 1; j < antenna->count; j++) {
                int ax = antenna->positions[i].x, ay = antenna->positions[i].y;
                int bx = antenna->positions[j].x, by = antenna->positions[j].y;

                int dx = bx - ax, dy = by - ay;

                int cx = ax - dx, cy = ay - dy;
                if (cx >= 0 && cx < rows && cy >= 0 && cy < cols) {
                    add_antinode(antinodes, size, cx, cy);
                }

                int dx2 = bx + dx, dy2 = by + dy;
                if (dx2 >= 0 && dx2 < rows && dy2 >= 0 && dy2 < cols) {
                    add_antinode(antinodes, size, dx2, dy2);
                }
            }
        }
    }
}

void get_antinodes_part2(Antenna *head, int rows, int cols, Antinode *antinodes, int *size) {
    for (Antenna *antenna = head; antenna != NULL; antenna = antenna->next) {
        if (antenna->count < 2) continue; // Skip if fewer than two antennas

        for (int i = 0; i < antenna->count; i++) {
            for (int j = i + 1; j < antenna->count; j++) {
                int ax = antenna->positions[i].x, ay = antenna->positions[i].y;
                int bx = antenna->positions[j].x, by = antenna->positions[j].y;

                int dx = bx - ax, dy = by - ay;

                add_antinode(antinodes, size, ax, ay);
                add_antinode(antinodes, size, bx, by);

                for (int k = 1; k < rows + cols; k++) {
                    int cx = ax - dx * k, cy = ay - dy * k;
                    if (cx >= 0 && cx < rows && cy >= 0 && cy < cols) {
                        add_antinode(antinodes, size, cx, cy);
                    }

                    int dx2 = bx + dx * k, dy2 = by + dy * k;
                    if (dx2 >= 0 && dx2 < rows && dy2 >= 0 && dy2 < cols) {
                        add_antinode(antinodes, size, dx2, dy2);
                    }
                }
            }
        }
    }
}

void free_antennas(Antenna *head) {
    while (head) {
        Antenna *temp = head;
        free(temp->positions);
        head = head->next;
        free(temp);
    }
}

void free_grid(char **grid, int rows) {
    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

int main() {
    char **grid;
    int rows, cols;

    read_input("input.txt", &grid, &rows, &cols);

    Antenna *antennas = NULL;
    get_antennas(grid, rows, cols, &antennas);

    Antinode antinodes1[rows * cols];
    int size1 = 0;
    get_antinodes(antennas, rows, cols, antinodes1, &size1);
    printf("Part 1 solution: %d\n", size1);

    Antinode antinodes2[rows * cols];
    int size2 = 0;
    get_antinodes_part2(antennas, rows, cols, antinodes2, &size2);
    printf("Part 2 solution: %d\n", size2);

    free_antennas(antennas);
    free_grid(grid, rows);

    return 0;
}
