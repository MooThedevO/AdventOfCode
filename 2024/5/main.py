from collections import defaultdict

def read_input(filename):
    with open(filename, 'r') as file:
        return file.readlines()

def input_parse(input):
    lines = [line.strip() for line in input]
    order, tests = [], []

    for line in lines:
        if '|' in line:
            numbers = [int(x) for x in line.split('|')]
            order.append(numbers)
        elif ',' in line:
            numbers = [int(x) for x in line.split(',')]
            tests.append(numbers)

    return order, tests

def order_pages(order):
    ordered_pages, rev_order = defaultdict(set), defaultdict(set)

    for x, y in order:
        ordered_pages[x].add(y)
        rev_order[y].add(x)

    return ordered_pages, rev_order

def order_tests(tests, order_pages):
    result, failed = [], []

    for test in tests:
        test_passed = True

        for idx in range(len(test) - 1):
            if test[idx + 1] not in order_pages[test[idx]]:
                test_passed = False
                failed.append(test)
                break

        if test_passed:
            result.append(test[len(test)//2])

    return sum(result), failed

def order_failed(tests, order_pages):
    order_failed_tests = []

    for test in tests:
        for idx in range(len(test) - 1):
            if test[idx + 1] not in order_pages[0][test[idx]]:
                while idx >= 0 and test[idx+1] in order_pages[1][test[idx]]:
                    test[idx+1], test[idx] = test[idx], test[idx+1]
                    
                    idx -= 1
                    
        order_failed_tests.append(test)
        
    return order_failed_tests

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 5\n")
    input = read_input('input.txt')
    
    pages, tests = input_parse(input)
    order = order_pages(pages)
    
    ordered, failed = order_tests(tests, order[0])
    print("Middle page number from correctly-ordered updates : ", ordered)

    ordered_f = order_failed(failed, order)
    failed_ordered, failed = order_tests(ordered_f, order[0])
    print("Middle page number from failed-ordered updates : ", failed_ordered)

if __name__ == '__main__':
    main()
