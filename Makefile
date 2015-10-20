CC = cc
CFLAGS = -c -O2 -Wall
DEBUG = -g
OBJS = dictionary.o trie.o parse.o

.PHONY: all debug clean

all: clean dictionary

debug: CFLAGS += $(DEBUG)	
debug: clean dictionary.dbg

clean:
	rm -f dictionary dictionary.dbg parse.h.gch trie.h.gch $(OBJS)

dictionary: $(OBJS)
	$(CC) -o dictionary $(OBJS)

dictionary.dbg: $(OBJS)
	$(CC) $(DEBUG) -o dictionary.dbg $(OBJS)

trie.o: trie.c trie.h
	$(CC) $(CFLAGS) trie.c trie.h

dictionary.o: dictionary.c parse.h trie.h
	$(CC) $(CFLAGS) dictionary.c

parse.o: parse.c parse.h
	$(CC) $(CFLAGS) parse.c parse.h
