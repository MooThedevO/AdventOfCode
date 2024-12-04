def read_input(filename):
    with open(filename, 'r') as file:
        return file.readlines()

def input_parse(input):
    return [line.strip() for line in input]

def count_all(input):
    word = "XMAS"
    rows, cols = len(input), len(input[0])
    directions = [
        (0, 1), (0, -1), (1, 0), (-1, 0), 
        (1, 1), (-1, -1), (1, -1), (-1, 1)
    ]
    count = 0
    for i in range(rows):
        for j in range(cols):
            for dx, dy in directions:
                if is_xmas_at_position(i, j, dx, dy, input, word, rows, cols):
                    count += 1
    return count

def is_xmas_at_position(x, y, dx, dy, input, word, rows, cols):
    for i in range(len(word)):
        nx, ny = x + i * dx, y + i * dy
        if not (0 <= nx < rows and 0 <= ny < cols) or input[nx][ny] != word[i]:
            return False
    return True

def count_xmas_x_shapes(input):
    rows, cols = len(input), len(input[0])
    count = 0
    for i in range(1, rows - 1):
        for j in range(1, cols - 1):
            if input[i][j] == 'A' and check_x_shape(i, j, input):
                count += 1
    return count

def check_x_shape(x, y, input):
    positive_direction = input[x-1][y-1] + input[x+1][y+1]
    negative_direction = input[x-1][y+1] + input[x+1][y-1]
    return any(x in positive_direction for x in ["MS", "SM"]) and any(x in negative_direction for x in ["MS", "SM"])

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 4\n")
    input = read_input('input.txt')
    print("Occurrences of XMAS : ", count_all(input_parse(input)))
    print("Occurrences of XMAS in X shape:", count_xmas_x_shapes(input))

if __name__ == '__main__':
    main()
