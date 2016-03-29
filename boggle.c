#include <stdio.h>
#include <stdlib.h>
#DEFINE STR_MIN 3
#DEFINE STR_MAX 16

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

    FILE *ifp = fopen("dictionary.txt", "r")

    fscanf(ifp, "%d",&numWords);

    for (i = 0; i < numWords; i++)
    {

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
    TrieNode *temp;

    if (root == NULL)
        root = createTrieNode();

    for (i = 0; i < length; i++)
    {
        index = tolower(string[i] - 'a');
        temp = temp->children[index];
    }

    temp->isWord = 1;

    return root;
}

void destroyTrie(TrieNode *root)
{
    int i;
    if (root == NULL)
        return;

    for (i = 0; i < 26; i++)
}
