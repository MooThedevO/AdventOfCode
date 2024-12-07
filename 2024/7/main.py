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

def test_numbers(result, numbers, operators, end):
    if len(numbers) == 0: 
        end[result] = True
        return 
    
    for operator in operators:
        new_result = apply_operator(result, numbers[0], operator)
        test_numbers(new_result, numbers[1:], operators, end)

def test_results(tests, numbers, operators):
    result = 0
    
    for test in range(len(tests)):
        results = {}
        results[tests[test]] = False
        
        test_numbers(0, numbers[test], operators, results)

        if results[tests[test]]:
            result += tests[test]

    return result

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 7\n")
    start = time.time()

    input = read_input('input.txt')
    tests, numbers = input_parse(input)
    
    print(test_results(tests, numbers, ['+', '*']))
    print(test_results(tests, numbers, ['+', '*', '||']))

    end = time.time()
    print(end - start)

if __name__ == '__main__':
    main() 