#include <stdio.h>
#include <stdlib.h>

typedef struct TrieNode {
	int isWord;
	struct TrieNode *character[26];
} TrieNode;

int main(void)
{

	return 0;
}

// Mallocs space and initializes a TrieNode.
TrieNode *createTrieNode(void)
{
	int i;
	TrieNode *newTrie = malloc(sizeof(TrieNode));

	newTrie->isWord = 0;

	for (i = 0; i < 26; i++)
		newTrie->character[i] = NULL;

	return newTrie;
}
