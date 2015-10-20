/* Author: Anna Prochowska
 * Date: April 2015
 * 
 * Reads the input, parses the commands and processes if are correct.
 * Manages data structure using radix tree which enables inserting 
 * and deleting words and queries if there is a word in a dictionary 
 * starting with a given prefix. 
 *
 * Run: make 
 * 		./dictionary
 * or: make debug
 * creates a file dictionary.dbg (compiled with flag -g for easier
 * memory leaks tracking)
 * 
 */

#include "trie.h"
#include "parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {false, true} bool;

extern const int LINE_LENGTH;

/* Number of nodes in the trie */
extern int trie_nodes;

/* Set to true if after a correct query number of nodes in trie should be
 * printed to stderr */
static bool print_trie_nodes;


/* Inserts a word to a dictionary and returns true if query is correct
 * and word doesn't exist in a dictionary; otherwise query is ignored and
 * function returns false. */
bool insert(char *params) {
    bool query_result = false;
    char *word = calloc(LINE_LENGTH, sizeof(char));
    int insert_result = -1;
    if (get_word_from_string(&params, word) && is_string_blank(&params)
        && (insert_result = trie_insert(word)) != -1) {
            printf("word number: %d\n", insert_result);
            query_result = true;
    }
    free(word);
    return query_result;
}

/* Inserts a fragment of a word to a dictionary and returns true
 * if parameters decribe correct word which doesn't exist in a dictionary;
 * otherwise query is ignored and function returns false. */
bool prev(char *params) {
    unsigned int number, start, end;
    int prev_result;
    if (get_number_from_string(&params, &number)
		&& get_number_from_string(&params, &start)
        && get_number_from_string(&params, &end) && is_string_blank(&params)
        && (prev_result = trie_prev(number, start, end)) != -1) {
            printf("word number: %d\n", prev_result);
            return true;
    }
    return false;
}

/* Deletes a word from a dictionary by its index and returns true if
 * the word was inserted, but wasn't deleted before; otherwise query 
 * is ignOred and function returns false. */
bool delete(char *params) {
    unsigned int number;
    if (get_number_from_string(&params, &number) && is_string_blank(&params)
        && trie_delete(number) != -1) {
            printf("deleted: %d\n", number);
            return true;
    }
    return false;
}


/* Checks if a pattern is a prefix of any word in a dictionary. 
 * Lookup result is printed to stdout. If the pattern and query are 
 * correct returns true; otherwise returns false. */
bool find(char *params) {
    char *pattern = calloc(LINE_LENGTH, sizeof(char));
    if (!get_word_from_string(&params, pattern)
		|| !is_string_blank(&params)) {
        	free(pattern);
        	return false;
    }
    puts(trie_find(pattern)?"YES":"NO");
    print_trie_nodes = false;
    free(pattern);
    return true;
}


/* Returns true if clear query is correct; false otherwise. Clears
 * dictionary data structure such that next inserted word has index 0. */
bool clear(char *params) {
    if (is_string_blank(&params)) { 
        trie_clear_and_init();
        puts("cleared");
        return true;
    }
    return false;
}

/* For a given input line, matches right command related to a dictionary
 * data structure. Returns true if query is correct and was executed 
 * properly; otherwise returns false. */
bool parse_query(char *query_text) {
    bool query_result = false;
    char *command = calloc(LINE_LENGTH, sizeof(char));
    get_word_from_string(&query_text, command);
    char commands[5][20] = {"insert", "prev", "delete", "find", "clear"};
    bool (*functions[])(char*) = {insert, prev, delete, find, clear};

    int i;
    for (i = 0; i < 5; i++)
        if (strcmp(command, commands[i]) == 0)
            query_result = functions[i](query_text);

    free(command);
    return query_result;
}

/* Deals with command line arguments and reads input. */
int main(int argc, char *argv[]) {
    bool debug_mode = false;
    char *line = NULL;
	size_t line_length = 0;
    trie_init();

    if (argc > 1) {
        if (argc == 2) {
            if (strcmp(argv[1],"-v") == 0)
                debug_mode = true;
            else
                puts("Error: bad argument");
        }
        else {
            puts("Error: bad arguments number");
        }
    }
    while (getline(&line, &line_length, stdin) != -1) {
        print_trie_nodes = true;
        line[strlen(line) - 1] = '\0';
        bool query_result = parse_query(line);

        if (query_result == false) 
            puts("ignored");
        else if (debug_mode == true && print_trie_nodes == true)
            fprintf(stderr, "nodes: %d\n", trie_nodes);
    }
    free(line);
    trie_clear();
    return 0;
}
