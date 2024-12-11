from collections import Counter

def read_input(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def parse_input(input):
    return input.strip().split(' ')

def blink(counts):
    new_count = Counter()

    for stone, count in counts.items():
        if len(stone) % 2 == 0:  
            half_len = len(stone) // 2
            left_half = stone[:half_len]
            right_half = stone[half_len:]

            new_count[left_half] += count
            new_count[str(int(right_half) if right_half != '0' else 0)] += count
        else:
            new_stone = str(int(stone) * 2024 if int(stone) != 0 else 1)
            new_count[new_stone] += count

    return new_count

def calc_blinks(count, times):
    for _ in range(times):  
        count = blink(count)
        
    return sum(count.values())

def main(): 
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 1\n")

    input = read_input('input.txt')
    stones = parse_input(input)
    count = Counter(stones)

    print('Part 1, 25 blinks : ', calc_blinks(count, 25))
    print('Part 2, 75 blinks : ', calc_blinks(count, 75))

if __name__ == '__main__':
    main()