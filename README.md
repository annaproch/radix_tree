Project for the course "Individual Programming Project" on the first year
of Computer Science at the University of Warsaw.

Dictionary with operations:
- insert word
returns index of an inserted word
- prev index start end
inserts substring of a word which is already in a dictionary
- delete index
deletes word with a given index from a dictionary
- find pattern
prints YES if there is a word in a dictionary with a given prefix, NO otherwise
- clear
clears a dictionary

Implementation of a compact prefix tree (radix trie).
Ignores incorrect commands.
If run with "-v" option, for each command (except "find") prints to stderr
number of nodes in a tree after executing the command.

Example of usage:
----------------

**in:**  
insert abcabc  
find bca  
prev 0 1 4  
find bca  
delete 10  
delete 1  
find bc  
clear  

*make  
./dictionary -v < in*

**Stdout**


word number: 0  
NO  
word number: 1  
YES  
ignored  
deleted: 1  
NO  
cleared  

**Stderr**


nodes: 2  
nodes: 3  
nodes: 2  
nodes: 0  
