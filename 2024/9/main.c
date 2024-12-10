#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_input(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* data = (char*)malloc(size + 1);
    if (!data) {
        perror("Memory allocation failed");
        exit(1);
    }

    fread(data, 1, size, file);
    data[size] = '\0';

    fclose(file);
    return data;
}

void input_parse(const char* input, long long** sequence, long long** location, long long** length, long long* seq_len) {
    long long file_id = 0;
    int is_file = 1;
    long long seq_pos = 0;
    long long seq_capacity = 1024;

    *sequence = (long long*)malloc(seq_capacity * sizeof(long long));
    *location = (long long*)malloc(seq_capacity * sizeof(long long));
    *length = (long long*)malloc(seq_capacity * sizeof(long long));

    if (!(*sequence) || !(*location) || !(*length)) {
        perror("Memory allocation failed");
        exit(1);
    }

    for (long long i = 0; i < strlen(input); i++) {
        char ch = input[i];

        if (is_file) {
            (*location)[file_id] = seq_pos;
            (*length)[file_id] = ch - '0';
            is_file = 0;
        } else {
            is_file = 1;
        }

        for (long long j = 0; j < (ch - '0'); j++) {
            if (seq_pos >= seq_capacity) {
                seq_capacity *= 2;
                *sequence = realloc(*sequence, seq_capacity * sizeof(long long));
                *location = realloc(*location, seq_capacity * sizeof(long long));
                *length = realloc(*length, seq_capacity * sizeof(long long));
            }
            (*sequence)[seq_pos++] = (i % 2 == 0) ? file_id : -1;
        }

        if (i % 2 == 0) {
            file_id++;
        }
    }
    *seq_len = seq_pos;
}

void move_files(long long* sequence, long long* seq_len) {
    long long new_length = 0;

    for (long long i = 0; i < *seq_len; i++) {
        if (sequence[i] != -1) {
            new_length++;
        }
    }

    long long* new_seq = (long long*)malloc(new_length * sizeof(long long));
    if (!new_seq) {
        perror("Memory allocation failed");
        exit(1);
    }

    long long new_index = 0;
    
    for (long long pos = 0; pos < new_length; pos++) {
        if (sequence[pos] == -1) {
            for (long long rpos = *seq_len - 1; rpos >= 0; rpos--) {
                if (sequence[rpos] != -1) {
                    new_seq[new_index++] = sequence[rpos];
                    sequence[rpos] = -1; 
                    break;
                }
            }
        } else {
            new_seq[new_index++] = sequence[pos];
        }
    }

    memcpy(sequence, new_seq, new_length * sizeof(long long));

    *seq_len = new_length;

    free(new_seq);
}

void move_files2(long long* sequence, long long* location, long long* length, long long* seq_len) {
    long long current_file = 0;

    while (length[current_file] > 0) {
        current_file++;
    }
    current_file--;

    for (long long to_move = current_file; to_move >= 0; to_move--) {
        long long free_space = 0;
        long long first_free = 0;

        while (first_free < location[to_move] && free_space < length[to_move]) {
            first_free = first_free + free_space;
            free_space = 0;

            while (sequence[first_free] != -1) {
                first_free++;
            }

            while (first_free + free_space < *seq_len && sequence[first_free + free_space] == -1) {
                free_space++;
            }
        }

        if (first_free >= location[to_move]) {
            continue;
        }

        for (long long i = first_free; i < first_free + length[to_move]; i++) {
            sequence[i] = to_move;
        }

        for (long long i = location[to_move]; i < location[to_move] + length[to_move]; i++) {
            sequence[i] = -1;
        }
    }
}

long long calculate_checksum(long long* sequence, long long seq_len) {
    long long checksum = 0;
    for (long long i = 0; i < seq_len; i++) {
        if (sequence[i] != -1) {
            checksum += i * sequence[i];
        }
    }
    return checksum;
}

int main() {
    printf("\nHello, world!\n");
    printf("Here's the solution to Advent of Code Day 9\n");

    char* input = read_input("input.txt");

    long long* sequence;
    long long* location;
    long long* length;
    long long seq_len;

    input_parse(input, &sequence, &location, &length, &seq_len);
    move_files(sequence, &seq_len);
    long long checksum1 = calculate_checksum(sequence, seq_len);

    input_parse(input, &sequence, &location, &length, &seq_len);
    move_files2(sequence, location, length, &seq_len);
    long long checksum2 = calculate_checksum(sequence, seq_len);

    printf("Part 1 result: %lld\n", checksum1);
    printf("Part 2 result: %lld\n", checksum2);

    free(input);
    free(sequence);
    free(location);
    free(length);

    return 0;
}
