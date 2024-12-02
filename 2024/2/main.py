def read_input(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def input_parse(input):
    list_input = [list(map(int, line.split())) for line in input.strip().split('\n')]
    return list_input

def is_safe_report(report): 
    is_increasing = is_decreasing = True
    
    for i in range(1, len(report)):
        diff = abs(report[i] - report[i-1])
        if diff < 1 or diff > 3:
            return False 
        
        if report[i] <= report[i-1]:
            is_increasing = False  
        
        if report[i] >= report[i-1]:
            is_decreasing = False  

    return is_increasing or is_decreasing

def is_safe_report_count(reports):
    safe_count = sum(1 for report in reports if is_safe_report(report)) 

    return safe_count

def is_safe_with_removal_count(reports):
    safe_count = 0

    for report in reports:
        for i in range(len(report)):
            new_report = report[:i] + report[i+1:]
            if is_safe_report(new_report):
                safe_count += 1
                break

    return safe_count

def main():
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 2\n")

    input = read_input('input.txt')
    print('number of safe reports : ', is_safe_report_count(input_parse(input)))
    print('number of safe reports with removal : ', is_safe_with_removal_count(input_parse(input)))

if __name__ == '__main__':
    main()
