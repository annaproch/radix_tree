#include "parse.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int LINE_LENGTH = 100010;

/* Returns 1 if string isn't empty and all characters are lowercase letters;
 * otherwise returns 0. */
int is_word_correct(char *word) {
    if (word[0] == '\0')
        return 0;
    while (*word) {
        if (islower(*word) == 0)
            return 0;
        word++;
    }
    return 1;
}

/* Returns 1 if string is not empty, all characters are digits, number is
 * without leading zeros and fits unsigned int type; otherwise returns 0. */
int is_number_string_correct(char *number) {
    if (number[0] == '\0'
		|| (strlen(number) > 1 && number[0] == '0')
        || strlen(number) > 10
		|| (strlen(number) == 10 && strcmp(number, "4294967296") >= 0))
        return 0;
    while (*number) {
        if (isdigit(*number) == 0)
            return 0;
        number++;
    }
    return 1;
}

int is_string_blank(char **line) {
    while (**line != '\0') {
        if (**line != ' ')
            return 0;
        (*line)++;
    }
    return 1;
}

/* Reads the first non-space character sequence from a string and sets
 * address to the string behind this sequence. Skips leading spaces. */
void get_string(char **line, char *word) {
    int i = 0;
    while (**line == ' ')
        (*line)++;

    while (**line && **line != ' ') {
        word[i] = **line;
        (*line)++;
        i++;
    }
    word[i] = '\0';
}

int get_word_from_string(char **line, char *word) {
    get_string(line, word);
    return is_word_correct(word);
}

int get_number_from_string(char **line, unsigned int *number) {
    char *number_string = calloc(LINE_LENGTH, sizeof(char));
    get_string(line, number_string);
    int is_number_correct = is_number_string_correct(number_string);
    *number = atoi(number_string);
    free(number_string);
    return is_number_correct;
}
