from collections import deque

def read_input(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def input_parse(input):
    return [list(map(int, line.strip())) for line in input.splitlines()]

dir = [(-1, 0), (1, 0), (0, -1), (0, 1)]

def bfs(start, map):
    rows, cols = len(map), len(map[0])

    queue = deque([start])
    visited = set()
    visited.add(start)

    reachable = 0

    while queue:
        x, y = queue.popleft()

        if map[x][y] == 9:
            reachable += 1

        for dx, dy in dir:
            nx, ny = x + dx, y + dy
            if 0 <= nx < rows and 0 <= ny < cols and (nx, ny) not in visited:
                if map[nx][ny] == map[x][y] + 1:
                    queue.append((nx, ny))
                    visited.add((nx, ny))

    return reachable

def dfs(x, y, map, visited, path):
    rows, cols = len(map), len(map[0])
    
    if x < 0 or x >= rows or y < 0 or y >= cols or (x, y) in visited:
        return 0  
    
    visited.add((x, y))
    
    if map[x][y] == 9:
        visited.remove((x, y))
        return 1  
    
    trail_count = 0
    for dx, dy in dir:
        nx, ny = x + dx, y + dy
        if 0 <= nx < rows and 0 <= ny < cols and map[nx][ny] == map[x][y] + 1:
            trail_count += dfs(nx, ny, map, visited, path)
    
    visited.remove((x, y)) 
    
    return trail_count

def calculate_scores(map):
    rows, cols = len(map), len(map[0])
    total_score = 0

    for i in range(rows):
        for j in range(cols):
            if map[i][j] == 0:
                score = bfs((i, j), map)
                total_score += score

    return total_score

def calculate_ratings(map):
    rows, cols = len(map), len(map[0])
    total_rating = 0

    for i in range(rows):
        for j in range(cols):
            if map[i][j] == 0:
                visited = set()
                rating = dfs(i, j, map, visited, [])
                total_rating += rating

    return total_rating

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 10\n")

    input = read_input('input.txt')
    map = input_parse(input)

    print("Sum of scores of all trailheads : ", calculate_scores(map))
    print("Sum of ratings of all trailheads : ", calculate_ratings(map))

if __name__ == '__main__':
    main()
