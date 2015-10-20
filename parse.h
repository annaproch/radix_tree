/* Author: Anna Prochowska
 * Date: April 2015
 * 
 * Parsing sequence of characters ended with '\0'. Reads non-space words
 * and numbers from the sequence and checks if they are correct.
 * 
 */

#ifndef PARSE
#define PARSE

/* Maximal length of input line */
extern const int LINE_LENGTH;

/* Returns 1 if string contains only spaces or is empty;
 * otherwise returns 0.
 * Increments string's address to the end or to the first non-space 
 * character. */
int is_string_blank(char **line);

/* Reads the first non-space character sequence from a string and sets
 * address to the string behind this sequence. Sets a pointer to char
 * parameter to this sequence. If it is a correct word returns 1;
 * otherwise returns 0. */
int get_word_from_string(char **line, char *word);

/* Reads the first non-space character sequence from a string and sets
 * address to the string behind this sequence. Sets a pointer to 
 * unsigned int parameter to number casted from this sequence.
 * If it is a correct number returns 1; otherwise returns 0. */
int get_number_from_string(char **line, unsigned int *number);

#endif
