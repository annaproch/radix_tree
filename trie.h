/* Author: Anna Prochowska
 * Date: April 2014
 * 
 * Implementation of a radix tree - compressed trie data structure. Merges
 * a parent node with its child if there is only one.
 * Enables inserting and deleting words from a dictionary and checking if
 * there exists a word with a given prefix in it.
 */

#ifndef TRIE
#define TRIE

#define INITIAL_DICT_SIZE 100
#define INITIAL_WORD_LIST_SIZE 100
#define empty_word_desc (Word_desc){0}

/* Description of a word in a dictionary. Consists of index of a word in 
 * an array with base words, and description of its fragment: index of
 * the first letter and word's length. */
struct Word_description {
    int index, start, length;
};
typedef struct Word_description Word_desc;

struct Node;
typedef struct Node* Tree;

/* Node in a radix tree. Consists of:
 * - description of a character sequence represented by node
 * - end_of_word variable set to 1 if there exists a word in a dictionary
 *   consisting of words represented by nodes on the path from the root to
 *   this node; set to 0 otherwise 
 * - an array of children nodes, i-th child points to a node with a word
 *   starting with i-th letter in alphabet (indexing from 0) */
struct Node {
    Tree *children;
    Word_desc word;
    int end_of_word;
};
typedef struct Node Node;

/* Counter of nodes in a tree. */
extern int trie_nodes;

/* Initializes empty data structure with all its components: trie,
 * base_words, word_list. */
void trie_init();

/* Inserts a word to data structure. Returns index of inserted word in
 * word_list or -1 if failed. */
int trie_insert(char *word);

/* Inserts fragment of a word to data structure if parametres describe
 * existing word. Returns 1 if inserting executed, 0 otherwise. */
int trie_prev(int word, int start, int end);

/* Frees all memory of data structure: trie, base_words and words_list. */
void trie_clear();

/* Returns 1 if there exists a word starting in a given node
 * and contained in node's subtree; returns 0 otherwise. */
int trie_find(char *word);

/* Deletes a word with a given index from a dictionary if index
 * describes an existing word. Returns 1 if deleting executed;
 * otherwise returns -1. */
int trie_delete(int number);

/* Frees data structure's memory and initializes new structure. */
void trie_clear_and_init();

#endif
