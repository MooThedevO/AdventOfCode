from collections import defaultdict
from itertools import combinations

def read_input(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def input_parse(input):
    return [line.strip() for line in input.splitlines()]

def get_antennas(lines):
    rows, cols = len(lines), len(lines[0])
    antennas = defaultdict(list)

    for row in range(rows):
        for col in range(cols):
            current_char = lines[row][col]

            if current_char == '.':
                continue
            if current_char not in antennas:
                antennas[current_char] = []
            
            antennas[current_char].append((row, col))

    return antennas

def get_antinodes_2(antennas, rows, cols):
    antinodes = set()

    for _, pos in antennas.items():
        for (ax, ay), (bx, by) in combinations(pos, 2):
            dx, dy = bx - ax, by - ay
            
            for i in range(max(rows, cols)):                
                cx, cy = ax - (dx*i) , ay - (dy*i)
                if 0 <= cx < rows and  0 <= cy < cols:
                    antinodes.add((cx, cy))
                
                cx, cy = bx + (dx*i) , by + (dy*i)
                if 0 <= cx < rows and  0 <= cy < cols:
                    antinodes.add((cx, cy))
                
    return antinodes

def get_antinodes(antennas, rows, cols):
    antinodes = set()

    for _, pos in antennas.items():
        for (ax, ay), (bx, by) in combinations(pos, 2):
            cx, cy = ax - (bx - ax), ay - (by - ay)
            if 0 <= cx < rows and  0 <= cy < cols:
                antinodes.add((cx, cy))
                
            dx, dy = bx + (bx - ax), by + (by - ay)
            if 0 <= dx < rows and  0 <= dy < cols:
                antinodes.add((dx, dy))
            
    return antinodes

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 2\n")

    input = read_input('input.txt')
    lines = input_parse(input)
    antennas = get_antennas(lines)
    
    print("Part 1 solution : ", len(get_antinodes(antennas, len(lines), len(lines[0]))))
    print("Part 2 solution : ", len(get_antinodes_2(antennas, len(lines), len(lines[0]))))

if __name__ == '__main__':
    main()