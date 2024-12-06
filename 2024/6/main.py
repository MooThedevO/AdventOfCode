import time

def read_input(filename):
    with open(filename, 'r') as file:
        return file.readlines()

def input_parse(input):
    return [[*line.strip()] for line in input]

def find_starting_pos(grid):
    for x in grid:
        for y in x:
            if y == '^':
                return (grid.index(x), x.index(y))
    return None

def move(grid, pos):
    cols, rows = len(grid), len(grid[0])
    directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]

    steps = 0
    current_direction = directions[0]
    current_position = pos

    while True:
        grid[current_position[0]][current_position[1]] = 'X'
        new_position = (current_position[0] + current_direction[0], current_position[1] + current_direction[1])

        if not (0 <= new_position[0] < cols and 0 <= new_position[1] < rows):
            steps += 1
            break
        
        if grid[new_position[0]][new_position[1]] == '#':
            current_direction = directions[(directions.index(current_direction) + 1) % 4]
            continue

        current_position = new_position

        if grid[current_position[0]][current_position[1]] != 'X':
            steps += 1

    return steps, grid

def will_loop(grid, pos):
    cols, rows = len(grid), len(grid[0])
    directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]  

    loops = 0
    current_position = pos
    current_direction = directions[0]

    visited = set()

    for x in range(cols):
        for y in range(rows):
            if grid[x][y] == 'X': 
                grid[x][y] = '#'
                
                visited.clear()

                position = current_position
                direction = current_direction

                while True:
                    if (position, direction) in visited:
                        loops += 1
                        break  
                    
                    visited.add((position, direction))

                    new_position = (position[0] + direction[0], position[1] + direction[1])

                    if not (0 <= new_position[0] < cols and 0 <= new_position[1] < rows):
                        break  
                    
                    if grid[new_position[0]][new_position[1]] == '#':
                        direction = directions[(directions.index(direction) + 1) % 4]
                        continue 
                    
                    position = new_position

                grid[x][y] = 'X'

    return loops, grid

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 6\n")
    start = time.time()

    input = read_input('input.txt')
    grid = input_parse(input)
    starting_pos = find_starting_pos(grid)

    print("Distinct positions will the guard visit before leaving the mapped area : ", move(grid, starting_pos)[0])
    print("Different positions that would cause a loop : ", will_loop(move(grid, starting_pos)[1], starting_pos)[0])

    end = time.time()
    print(end - start)

if __name__ == '__main__':
    main()
