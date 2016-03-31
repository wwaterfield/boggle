#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define STR_MIN 3
#define STR_MAX 16
#define BOARD_SIZE 4

// For DX and DY Arrays.
const int DX_SIZE = 8;
const int DX[] = {-1,-1,-1,0,0,1,1,1};
const int DY[] = {-1,0,1,-1,1,-1,0,1};

typedef struct TrieNode {
	int isWord;
	struct TrieNode *character[26];
} TrieNode;

TrieNode *createTrieNode(void);
TrieNode *insertString(TrieNode *root, char *string);
void freeTrie(TrieNode *root);
char **readBoard(void);
void print2DArray(char **board);
int inBounds(int newX, int newY);

int main(void)
{
    // Declaring variables.
    int numWords, numCases, i, len;
    char temp[17];
    char **board;

    // Initializing the root node of the trie.
    TrieNode *root = createTrieNode();

    FILE *ifp = fopen("dictionary.txt", "r");

    fscanf(ifp, "%d",&numWords);

    // Adds each word from the dictionary to the trie.
    for (i = 0; i < numWords; i++)
    {
        fscanf(ifp, "%s", temp);
        len = strlen(temp);
        if (len >= 3 && len <= 16)
            root = insertString(root, temp);
    }

    // Process each game individually.
    scanf("%d", &numCases);

    for (i = 0; i < numCases; i++)
    {
        // Read in each game board.
        board = readBoard();

        //Debug print board.
        print2DArray(board);
    }



	return 0;
}

// Mallocs space and initializes a TrieNode.
TrieNode *createTrieNode(void)
{
	int i;
	TrieNode *newTrie = malloc(sizeof(TrieNode));

    if (newTrie == NULL)
        exit(-1);
	newTrie->isWord = 0;

	for (i = 0; i < 26; i++)
		newTrie->character[i] = NULL;

	return newTrie;
}

TrieNode *insertString(TrieNode *root, char *str)
{
    int i, index;
    int length = strlen(str);
    TrieNode *temp = root;

    if (root == NULL)
        root = temp = createTrieNode();

    //printf("Current String: %s\n", str);
    for (i = 0; i < length; i++)
    {
        index = tolower(str[i] - 'a');

        if (temp->character[index] == NULL)
            temp->character[index] = createTrieNode();

        temp = temp->character[index];
    }

    temp->isWord = 1;

    return root;
}

void freeTrie(TrieNode *root)
{
    int i;
    if (root == NULL)
        return;

    for (i = 0; i < 26; i++)
        freeTrie(root->character[i]);
    free(root);
}

char **readBoard(void)
{
    int i, j;
    char letter;
    char **board = malloc(sizeof(char*) * BOARD_SIZE);

	for (i = 0; i < BOARD_SIZE; i++)
		board[i] = malloc(sizeof(char) * BOARD_SIZE+1);

	for (i = 0; i < BOARD_SIZE; i++)
		scanf("%s", board[i]);

    return board;
}

void print2DArray(char **board)
{
    int i, j;

    for (i = 0; i < BOARD_SIZE; i++)
    {
        for (j = 0; j < BOARD_SIZE; j++)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int inBounds(int newX, int newY)
{
	if (newX > BOARD_SIZE - 1 || newY > BOARD_SIZE -1)
		return 0;
	else return 1;

}



// Need is Prefix Function
