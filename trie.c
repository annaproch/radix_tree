#include "trie.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Array of words inserted to a dictionary explicitely, not by describing 
 * extract of a word inserted before. */
char **base_words;
/* Number of words in base_words. */
int base_words_count;
/* Size of base_words, for how many char pointers memory is allocated. */
int base_words_size;

/* Array of descriptions of words in a dictionary. 
 * If i-th word was deleted word_list[i] is an empty pointer. */
Word_desc **word_list;
/* Number of words inserted to dictionary since last initialization of data
 * structure. Doesn't change when word is deleted. */
int words_count;
/* Size of word_list, for how many char pointers memory is allocated. */
int word_list_size;

/* Root of a radix tree. */
Tree root;
/* Counter of nodes in a tree. */
int trie_nodes;


void trie_init() {
    root = 0;
    base_words_size = INITIAL_DICT_SIZE;
    word_list_size = INITIAL_WORD_LIST_SIZE;
    base_words = calloc(base_words_size, sizeof(char *));
    word_list = calloc(word_list_size, sizeof(Word_desc*));
    words_count = 0;
    trie_nodes = 0;
    base_words_count = 0;
}

/* Adds new word to word_list. Allocates more memory if needed. */
void add_to_word_list(Word_desc desc) {
    if (word_list_size <= words_count) {
        word_list_size *= 2;
        word_list = realloc(word_list, word_list_size*sizeof(Word_desc));
        memset(word_list + word_list_size/2, 
        0, 
        word_list_size/2*sizeof(char *));
    }
    Word_desc *new_desc = malloc(sizeof(Word_desc));
    *new_desc = desc;
    word_list[words_count++] = new_desc; 
}

/* Adds new word to base_words. Allocates more memory if needed. */
void add_to_base_words(char *word) {
    if (base_words_size <= base_words_count) {
        base_words_size *= 2;
        base_words = realloc(base_words, base_words_size*sizeof(char *));
        memset(base_words+base_words_size/2,
        0,
        base_words_size/2*sizeof(char *));
    }
    char *new_word = calloc(strlen(word)+1, sizeof(char));
    int i = 0;
    while(word[i] != '\0') {
        new_word[i] = word[i];
        i++;
    }
    base_words[base_words_count++] = new_word;
}

/* Finds the length of a common prefix of two words. Paremetres
 * are decriptions of those words. Works in linear time. */
int common_prefix_by_desc(Word_desc fst, Word_desc snd) {
    int common_length = 0;
    while(common_length < fst.length && common_length < snd.length
        && base_words[fst.index][fst.start+common_length] == 
        base_words[snd.index][snd.start+common_length])
        common_length++;
    return common_length;
}

/* Finds the first letter of a word by its description.
 * Returns number of a letter from the beginning of alphabet,
 * if 'a' returns 0, etc. */
int first_letter(Word_desc desc) {
    return base_words[desc.index][desc.start]-'a';
}

/* Recursive function to insert word to a tree, starting from
 * the given node. Word's decription is given.
 * Allocates memory for new nodes. */
int trie_insert_recur(Tree *trie, Word_desc desc) {
    if (*trie == 0) {
        Word_desc insert_desc = (*trie == root)?empty_word_desc:desc;
        *trie = malloc(sizeof(struct Node));
        **trie = (struct Node){calloc(26, sizeof(Tree)), insert_desc, 1};
        trie_nodes++;
        if (*trie == root) {
        Tree *matching_child = &(*trie)->children[first_letter(desc)];
            return trie_insert_recur(matching_child, desc);
        }
        else {
        return words_count - 1;
        }
    }
    int common = common_prefix_by_desc(desc, (*trie)->word);
    if (common < (*trie)->word.length) {
        Word_desc new_word = {(*trie)->word.index,
			(*trie)->word.start,
			common};
        Tree new_child = *trie;
        new_child->word.start += common;
        new_child->word.length -= common;
        Tree new_node = malloc(sizeof(struct Node));
        *new_node = (struct Node){calloc(26, sizeof(Tree)), new_word, 0};
        new_node->children[first_letter((*new_child).word)] = new_child;
        *trie = new_node;
        trie_nodes++;
    }
    if (common == desc.length) {
        if ((*trie)->end_of_word == 1)
        return -1;
        (*trie)->end_of_word = 1;
        return words_count - 1;
    } else {
        Word_desc new_desc = {desc.index,
			desc.start+common,
			desc.length-common};
		Tree *matching_child = &(*trie)->children[first_letter(new_desc)];
        return trie_insert_recur(matching_child, new_desc);
    }
    return -1;
}

int trie_insert(char *word) {
    add_to_base_words(word);
    Word_desc desc = {base_words_count-1, 0, strlen(word)};
    add_to_word_list(desc);

    int insert_res =  trie_insert_recur(&root, desc);
    if (insert_res == -1) {
        free(base_words[--base_words_count]);
        base_words[base_words_count] = 0;
        free(word_list[--words_count]);
        word_list[words_count] = 0;
    }
    return insert_res;
}

/* The length of the longest common prefix of two words. One is 
* given by char pointer, while the other one by Word_desc. */
int common_substr(Word_desc desc, char *word) {
    int iter = desc.start;
    while(iter < desc.start + desc.length && *word != '\0'
        && *word == base_words[desc.index][iter]) {
        iter++;
        word++;
    }    
    return iter - desc.start;
}

/* Returns 1 if there exists a word starting in a given node
 * and contained in node's subtree; returns 0 otherwise. */
int trie_find_recur(Tree trie, char *word) {
    if (trie == 0)
        return 0;
    int common = common_substr(trie->word, word);
    if (common == strlen(word)) {
        return 1;
    }
    if (common < trie->word.length)
        return 0;
    return trie_find_recur(trie->children[*(word+common)-'a'], word+common);
}


int trie_find(char *word) {
    return trie_find_recur(root, word);
}

/* Returns a pointer to a non-empty child node or NULL if such
 * doesn't exist. */
Tree find_child(Tree trie) {
    int i;
    for (i = 0; i < 26; i++)
        if (trie->children[i] != 0)
            return trie->children[i];
    return NULL;
}

/* Returns a number of non-empty children nodes. */
int number_of_children(Tree trie) {
    int i;
    int res = 0;
    for (i = 0; i < 26; i++)
        if (trie->children[i] != 0)
            res++;
    return res;
}

/* Frees memory of every node in a tree. */
void free_tree(Tree trie) {
    if (trie == 0)
        return;
    int i;
    for (i = 0; i < 26; i++) {
        if (trie->children[i] != NULL) {
            free_tree(trie->children[i]);
        }
    }
    free(trie->children);
    free(trie);
    trie_nodes--;
}

/* Deletes recursively a word from a tree if there exists one.
 * Frees memory of every deleted node. Merges a node with
 * its child if it has only one. */ 
int trie_delete_recur(Tree *trie, Word_desc desc) {
    if (*trie == 0)
        return -1;
    int word_node_length = (*trie)->word.length;
    int delete_result = -1;
    if (word_node_length != desc.length) {
        Word_desc new_desc = desc;
        new_desc.start += word_node_length;
        new_desc.length -= word_node_length;
		Tree *matching_child = &(*trie)->children[first_letter(new_desc)];
        delete_result = trie_delete_recur(matching_child, new_desc);
        if (delete_result == -1)
            return -1;
    }
	int children_number = number_of_children(*trie);
    if (*trie != root) {
        if (children_number) {
            if (word_node_length == desc.length) {
                (*trie)->end_of_word = 0;
            }
            if (children_number == 1 && (*trie)->end_of_word == 0) {
                Tree only_child = find_child(*trie);
                trie_nodes--;
                free((*trie)->children);
                free(*trie);
                *trie = only_child;
                (*trie)->word.start -= word_node_length;
                (*trie)->word.length += word_node_length;
            }
        }
        else {
            if ((delete_result != -1 && (*trie)->end_of_word == 0)
				|| delete_result == -1) {
                free_tree(*trie);
                *trie = 0;
            }
        }
    } else {
      if (number_of_children(*trie) == 0) {
        free_tree(*trie);
        *trie = 0;
      }
    }
    return 1;
}

int trie_delete(int number) {
    int delete_result = -1;
    if (number < 0 || number >= words_count || word_list[number] == 0)
        return -1;
    delete_result = trie_delete_recur(&root, *word_list[number]);
    if (delete_result != -1) {
        free(word_list[number]);
        word_list[number] = 0;
    }
    return delete_result;
}

int trie_prev(int index, int start, int end) {
    if (index < 0 || index >= words_count || start > end
		|| word_list[index] == 0 || word_list[index]->length <= end
		|| start < 0)
            return -1;
    
    Word_desc desc = {word_list[index]->index,
                    start+word_list[index]->start,
                    end-start+1};
    add_to_word_list(desc);

    int prev_res =  trie_insert_recur(&root, desc);
    if (prev_res == -1) {
        free(word_list[--words_count]);
        word_list[words_count] = 0;
    }
    return prev_res;
}

void trie_clear() {
    free_tree(root);    
    int i;
    for (i = 0; i < base_words_size; i++)
        if (base_words[i] != 0)
            free(base_words[i]);
    for (i = 0; i < word_list_size; i++)
        if (word_list[i])
            free(word_list[i]);
    free(base_words);
    free(word_list);
}

void trie_clear_and_init() {
    trie_clear();
    trie_init();
}
