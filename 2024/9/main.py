def read_input(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def input_parse(input):
    file_id = 0
    is_file = True
    
    sequence = []
    location, length = [0] * len(input ), [0] * len(input)

    for pos, char in enumerate(input):
        if is_file:
            location[file_id] = len(sequence)
            length[file_id] = int(char)
            is_file = False
        else:
            is_file = True

        for _ in range(int(char)):
            sequence.append(str(file_id) if pos % 2 == 0 else '.')

        if pos % 2 == 0:
            file_id += 1

    return sequence, location, length

def move_files(sequence):
    new_seq = []
    new_length = len(sequence) - sequence.count('.')

    for pos in range(new_length):
        if sequence[pos] == '.':
            for rpos in range(len(sequence)-1, 0, -1):
                if sequence[rpos]!= '.':
                    new_seq.append(sequence[rpos])
                    sequence[rpos] = '.'
                    break
        else:
            new_seq.append(sequence[pos])   

    return new_seq

def move_files2(sequence_list):
    sequence, location, length = sequence_list

    current_file = 0
    while length[current_file] > 0:
        current_file += 1
    current_file -= 1

    for to_move in range(current_file, -1, -1):
        free_space = 0
        first_free = 0

        while first_free < location[to_move] and free_space < length[to_move]:
            first_free = first_free + free_space
            free_space = 0
            while sequence[first_free] != '.':
                first_free += 1
            while first_free + free_space < len(sequence) and sequence[first_free + free_space] == '.':
                free_space += 1

        if first_free >= location[to_move]:
            continue

        for i in range(first_free, first_free + length[to_move]):
            sequence[i] = to_move
        for i in range(location[to_move], location[to_move] + length[to_move]):
            sequence[i] = '.'

    return sequence

def calculate_checksum(sequence):
    checksum = 0

    for i, file in enumerate(sequence):
        if file != '.':
            checksum += i * int(file)

    return checksum

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 9\n")

    input = read_input('input.txt')
    
    files = move_files(input_parse(input)[0])
    files2 = move_files2(input_parse(input))

    print("Part 1 result : ", calculate_checksum(files))
    print("Part 2 result : ", calculate_checksum(files2))
    
if __name__ == '__main__':
    main()