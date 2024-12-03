#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000

char* read_input(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(size + 1);
    if (!buffer) {
        perror("Unable to allocate memory");
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, size, file);
    fclose(file);

    buffer[size] = '\0'; 
    return buffer;
}

void input_parse_all(const char* input, int (*mul_results)[2], int* count) {
    const char* ptr = input;
    while (*ptr) {
        if (strncmp(ptr, "mul(", 4) == 0) {
            ptr += 4;  
            int num1 = 0, num2 = 0;
            while (isdigit(*ptr)) {
                num1 = num1 * 10 + (*ptr - '0');
                ptr++;
            }
            if (*ptr == ',') {
                ptr++; 
            }
            while (isdigit(*ptr)) {
                num2 = num2 * 10 + (*ptr - '0');
                ptr++;
            }
            if (*ptr == ')') {
                ptr++; 
                mul_results[*count][0] = num1;
                mul_results[*count][1] = num2;
                (*count)++;
            }
        } else {
            ptr++;
        }
    }
}

int calculate_multiplications(int mul_results[][2], int count) {
    int total = 0;
    for (int i = 0; i < count; i++) {
        total += mul_results[i][0] * mul_results[i][1];
    }
    return total;
}

void input_parse_sub(const char* input, int (*mul_results)[2], int* count) {
    const char* ptr = input;
    int skip_do = 0;  

    while (*ptr) {
        if (strncmp(ptr, "don\'t()", 7) == 0) {
            skip_do = 1;
            ptr += 8;
        }
        else if (strncmp(ptr, "do()", 4) == 0) {
            skip_do = 0;  
            ptr += 4;
        }
        else if (strncmp(ptr, "mul(", 4) == 0) {
            if (!skip_do) {
                ptr += 4;  
                int num1 = 0, num2 = 0;
                while (isdigit(*ptr)) {
                    num1 = num1 * 10 + (*ptr - '0');
                    ptr++;
                }
                if (*ptr == ',') {
                    ptr++; 
                }
                while (isdigit(*ptr)) {
                    num2 = num2 * 10 + (*ptr - '0');
                    ptr++;
                }
                if (*ptr == ')') {
                    ptr++;
                    mul_results[*count][0] = num1;
                    mul_results[*count][1] = num2;
                    (*count)++;
                }
            } else {
                while (*ptr && *ptr != ')') {
                    ptr++;
                }
                if (*ptr == ')') {
                    ptr++;
                }
            }
        } else {
            ptr++;
        }
    }
}

int main() {
    char* input = read_input("input.txt");

    int mul_results_all[1000][2];
    int count_all = 0;
    input_parse_all(input, mul_results_all, &count_all);
    
    int result_part_one = calculate_multiplications(mul_results_all, count_all);
    printf("Result for all mul(x,y): %d\n", result_part_one);

    int mul_results_sub[1000][2];
    int count_sub = 0;
    input_parse_sub(input, mul_results_sub, &count_sub);
    
    int result_part_two = calculate_multiplications(mul_results_sub, count_sub);
    printf("Result for mul(x,y) respecting do/don't constraints: %d\n", result_part_two);

    free(input);

    return 0;
}
