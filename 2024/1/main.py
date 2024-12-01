def read_input(filename):
    with open(filename, 'r') as file:
        data = file.read()
    return data

def input_parse(input):
    list_input = [[int(num) for num in line.split()] for line in input.strip().split('\n')]
    list1 = sorted([item[0] for item in list_input])
    list2 = sorted([item[1] for item in list_input])
    return list1, list2

def calculate_difference(list1, list2):
    result = sum([abs(list1[i] - list2[i]) for i in range(len(list1))])
    return result
def calculate_similarity(list1, list2):
    result = sum([list2.count(i)*i for i in list1])
    return result

def main(): 
    print("\nHello, world!\n")
    print("Here's the solution to Advent of Code Day 1\n")

    input = read_input('input.txt')
    print('total distance is : ', calculate_difference(*input_parse(input)))
    print('similarity score is : ', calculate_similarity(*input_parse(input)))

if __name__ == '__main__':
    main()