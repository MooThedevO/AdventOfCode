import time

def read_input(filename):
    with open(filename, 'r') as file:
        return file.readlines()

def input_parse(input):
    lines = [line.strip().split(":") for line in input]
    tests, numbers = [], []
    
    for line in lines:
        tests.append(int(line[0]))
        numbers.append([int(num) for num in line[1].split()])

    return tests, numbers

def apply_operator(n1, n2, operator):
    if operator == '+':
        return n1 + n2
    elif operator == '*':
        return n1 * n2
    elif operator == '||':
        return int(str(n1) + str(n2)) 

def test_numbers(numbers, operators):
    possible_results = {numbers[0]}  # Start with the first number
    
    for i in range(1, len(numbers)):
        new_results = set()
        for operator in operators:
            for result in possible_results:
                new_results.add(apply_operator(result, numbers[i], operator))
        possible_results = new_results  # Update possible results for the next number

    return possible_results

def test_results(tests, numbers, operators):
    result = 0
    
    for test, nums in zip(tests, numbers):
        possible_results = test_numbers(nums, operators)
        
        if test in possible_results:
            result += test

    return result

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 7\n")
    start = time.time()

    input = read_input('input.txt')
    tests, numbers = input_parse(input)
    
    result_part1 = test_results(tests, numbers, ['+', '*'])
    result_part2 = test_results(tests, numbers, ['+', '*', '||'])

    print("Part 1 Result:", result_part1)
    print("Part 2 Result:", result_part2)

    end = time.time()
    print("Execution time:", end - start)

if __name__ == '__main__':
    main()
