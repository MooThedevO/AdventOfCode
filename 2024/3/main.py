import re

def read_input(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def input_parse_all(input):
    pattern = r'mul\((\d{1,3}),\s*(\d{1,3})\)'
    all_matches = re.findall(pattern, input)
    
    return all_matches

def input_parse_sub(input):
    digit_pattern = r'mul\((\d{1,3}),\s*(\d{1,3})\)'
    do_pattern = r'do\(\)|don\'t\(\)'

    all_matches = sorted(re.finditer(f'{digit_pattern}|{do_pattern}', input), key=lambda m: m.start())

    valid_matches = []
    skip_do = False

    for match in all_matches:
        if match.group() == "don\'t()":
            skip_do = True
        elif match.group() == "do()":
            skip_do = False
        elif not skip_do:
            valid_matches.append(match.group())
            
    return valid_matches

def calculate_multiplications(numbers):
    result = 0
    for num in numbers:
        try:
            result += int(num[0]) * int(num[1])
        except:
            continue
    return result

def calculate_dont_multiplications(matches):
    result = 0
    for match in matches:
        numbers = re.findall(r'\d{1,3}', match)
        result += int(numbers[0]) * int(numbers[1])

    return result

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 3\n")

    input = read_input('input.txt')

    print("Result for all mul(x,y) : ", calculate_multiplications(input_parse_all(input)))
    print("Result for all mul(x,y) respecing do/don't constraints : ",calculate_dont_multiplications(input_parse_sub(input)))

if __name__ == '__main__':
    main()
