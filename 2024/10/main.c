#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX_ROWS 100
#define MAX_COLS 100

int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

void read_input(const char *filename, int map[MAX_ROWS][MAX_COLS], int *rows, int *cols) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("File opening failed");
        return;
    }
    
    *rows = 0;
    char line[MAX_COLS + 1];  
    while (fgets(line, sizeof(line), file)) {
        int col = strlen(line) - 1;  
        if (*rows == 0) {
            *cols = col;  
        }
        for (int i = 0; i < col; i++) {
            map[*rows][i] = line[i] - '0';
        }
        (*rows)++;
    }
    
    fclose(file);
}

int bfs(int startX, int startY, int map[MAX_ROWS][MAX_COLS], int rows, int cols) {
    int reachable = 0;
    int queue[MAX_ROWS * MAX_COLS][2];
    int front = 0, back = 0;
    
    int visited[MAX_ROWS][MAX_COLS] = {0};
    queue[back][0] = startX;
    queue[back][1] = startY;
    back++;
    visited[startX][startY] = 1;

    while (front < back) {
        int x = queue[front][0];
        int y = queue[front][1];
        front++;

        if (map[x][y] == 9) {
            reachable++;
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + directions[i][0];
            int ny = y + directions[i][1];

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && !visited[nx][ny] && map[nx][ny] == map[x][y] + 1) {
                queue[back][0] = nx;
                queue[back][1] = ny;
                back++;
                visited[nx][ny] = 1;
            }
        }
    }

    return reachable;
}

int dfs(int x, int y, int map[MAX_ROWS][MAX_COLS], int rows, int cols, int visited[MAX_ROWS][MAX_COLS]) {
    if (x < 0 || x >= rows || y < 0 || y >= cols || visited[x][y]) {
        return 0;
    }

    visited[x][y] = 1;

    if (map[x][y] == 9) {
        visited[x][y] = 0;
        return 1;
    }

    int trail_count = 0;
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && map[nx][ny] == map[x][y] + 1) {
            trail_count += dfs(nx, ny, map, rows, cols, visited);
        }
    }

    visited[x][y] = 0;
    return trail_count;
}

int calculate_scores(int map[MAX_ROWS][MAX_COLS], int rows, int cols) {
    int total_score = 0;
    int visited[MAX_ROWS][MAX_COLS] = {0};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == 0 && !visited[i][j]) {
                int score = bfs(i, j, map, rows, cols);
                total_score += score;
            }
        }
    }

    return total_score;
}

int calculate_ratings(int map[MAX_ROWS][MAX_COLS], int rows, int cols) {
    int total_rating = 0;
    int visited[MAX_ROWS][MAX_COLS] = {0};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j] == 0 && !visited[i][j]) {
                total_rating += dfs(i, j, map, rows, cols, visited);
            }
        }
    }
    return total_rating;
}

int main() {
    int map[MAX_ROWS][MAX_COLS];
    int rows, cols;
    
    read_input("input.txt", map, &rows, &cols);

    printf("Sum of scores of all trailheads: %d\n", calculate_scores(map, rows, cols));
    printf("Sum of ratings of all trailheads: %d\n", calculate_ratings(map, rows, cols));

    return 0;
}
