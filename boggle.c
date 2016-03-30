#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define STR_MIN 3
#define STR_MAX 16

typedef struct TrieNode {
	int isWord;
	struct TrieNode *character[26];
} TrieNode;

TrieNode *createTrieNode(void);
TrieNode *insertString(TrieNode *root, char *string);


int main(void)
{
    int numWords, i;
    char temp[17];

    TrieNode *root = createTrieNode();
    FILE *ifp = fopen("sampledictionary.txt", "r");

    fscanf(ifp, "%d",&numWords);

    for (i = 0; i < numWords; i++)
    {
        fscanf(ifp, "%s", temp);
        root = insertString(root, temp);
    }


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

TrieNode *insertString(TrieNode *root, char *string)
{
    int i, index;
    int length = strlen(string);
    TrieNode *temp = root;

    if (root == NULL)
        root = temp = createTrieNode();

    printf("Current String: %s\n", string);
    for (i = 0; i < length; i++)
    {
        index = tolower(string[i] - 'a');
        printf("index: %d\n", index);
        temp = temp->character[index];
        printf("Added %c\n" ,index + 'a');
    }

    temp->isWord = 1;

    return root;
}

/*
void destroyTrie(TrieNode *root)
{
    int i;
    if (root == NULL)
        return;

    for (i = 0; i < 26; i++)
} */
