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

void findWords(TrieNode *root, int **visitedArray, char **board, int curX, int curY, int k, char *string);
TrieNode *createTrieNode(void);
TrieNode *insertString(TrieNode *root, char *string);
int isPrefix(TrieNode *root, char *str);
int isWord(TrieNode *root, char *str);
char **readBoard(void);
int inBounds(int newX, int newY);
int **visitedArray(void);
void freeTrie(TrieNode *root);
void free2DCharArray(char **array, int rows);
void free2DIntArray(int **array, int rows);

int main(void)
{
    // Declaring variables.
    int numWords, numCases, i,j,k, len, **visited;
    char temp[STR_MAX+1];
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
        if (len >= STR_MIN && len <= STR_MAX)
            root = insertString(root, temp);
    }

    // Process each game individually.
    scanf("%d", &numCases);

    for (i = 0; i < numCases; i++)
    {
        char string[STR_MAX+1];
        string[0] = '\0';

        // Read in the game board.
        board = readBoard();

		// Generate a visited array.
		visited = visitedArray();


        printf("Words for Game #%d:\n", i+1);

		// Finds words in the boggle puzzle and prints them out.
		for (j = 0; j < BOARD_SIZE; j++)
		{
            for (k = 0; k < BOARD_SIZE; k++)
            {
                findWords(root, visited, board, j, k, 0, string);
                free2DIntArray(visited, BOARD_SIZE);
                visited = visitedArray();
            }

		}

		printf("\n\n");

		// Freeing up memory.
		free2DIntArray(visited, BOARD_SIZE);
		free2DCharArray(board, BOARD_SIZE);
    }

    // Free the trie.
    freeTrie(root);
    fclose(ifp);

	return 0;
}

void findWords(TrieNode *root, int **visitedArray, char **board, int curX, int curY, int k, char *string)
{
    // Declaring variables.
	int i, newX, newY;

    // Adding the current letter to the end of the string.
	string[k] = board[curX][curY];
	string[k+1] = '\0';
	visitedArray[curX][curY]=-1;

	// If it's a prefix, also check if it's a word.
    if (isPrefix(root, string))
    {
        // Mark the visited array and check if string is a word.
        visitedArray[curX][curY] = 1;

        if(isWord(root, string))
            printf("%s\n", string);
    }
    else
    {
        visitedArray[curX][curY] = -1;
        return;
    }

    // Look through all of the adjacent characters in the board.
    for (i = 0; i < DX_SIZE; i++)
    {
        // Set newX and newY
        newX = curX + DX[i];
        newY = curY + DY[i];

        // If newX or newY are out of bounds,
        // move on to the next iteration.
        if (inBounds(newX, newY) == 0 || visitedArray[newX][newY] != -1)
            continue;

        findWords(root, visitedArray, board, newX, newY, k+1, string);
    }

    // Once a path has been checked, reset the visited array.
    visitedArray[curX][curY] = -1;
    return;
}

// Mallocs space and initializes a TrieNode.
TrieNode *createTrieNode(void)
{
	int i;
	TrieNode *newTrie = malloc(sizeof(TrieNode));

    // If malloc fails, exit the program.
    if (newTrie == NULL)
        exit(-1);

    // Initialize isWord.
	newTrie->isWord = 0;

    // Initialize each child node.
	for (i = 0; i < 26; i++)
		newTrie->character[i] = NULL;

	return newTrie;
}

// Inserts a given string into the trie structure.
TrieNode *insertString(TrieNode *root, char *str)
{
    int i, index;
    int length = (int)strlen(str);
    TrieNode *temp = root;

    // If root doesn't exist, create a node for root.
    if (root == NULL)
        root = temp = createTrieNode();

    // Go through the trie, making nodes when necessary.
    for (i = 0; i < length; i++)
    {
        index = tolower(str[i]) - 'a';

        if (temp->character[index] == NULL)
            temp->character[index] = createTrieNode();

        temp = temp->character[index];
    }

    // Change the last node to indicate a word.
    temp->isWord = 1;

    return root;
}

// Determines if a given string is a prefix in the trie.
int isPrefix(TrieNode *root, char *str)
{
	int i, index;
	int length = strlen(str);
	TrieNode *temp = root;

    // If we don't hit a NULL, the string must be a word or prefix.
	for (i = 0; i < length; i++)
	{
		index = tolower(str[i]) - 'a';

		if (temp->character[index] == NULL)
			return 0;
		temp = temp->character[index];
	}

	return 1;
}

// Determines if a given string is a word in the dictionary.
int isWord(TrieNode *root, char *str)
{
	int i, index;
	int length = strlen(str);
	TrieNode *temp = root;

	// Traverse through the trie.
	for (i = 0; i < length; i++)
	{
		index = tolower(str[i]) - 'a';

		if(temp->character[index] == NULL)
		{
            //printf("Not Found: %c\n", index + 'a');
			return 0;
        }
		temp = temp->character[index];
	}

	// Return if the string is a word.
	if (temp->isWord == 1)
		return 1;
}

// Scans in the Boggle board from stdin and
// returns a 2D array with the input.
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


// Determines if a given set of coordinates is out of bounds
int inBounds(int newX, int newY)
{
	if (newX > BOARD_SIZE - 1 || newX < 0 || newY > BOARD_SIZE -1 || newY < 0)
		return 0;
	else return 1;
}

// Mallocs and initializes a visited array.
int **visitedArray(void)
{
	int i,j;
	int **visited = malloc(sizeof(int*) * BOARD_SIZE);

	for (i = 0; i < BOARD_SIZE; i++)
		visited[i] = malloc(sizeof(int) * BOARD_SIZE);

	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			visited[i][j] = -1;

	return visited;
}

// Recursively frees a trie.
void freeTrie(TrieNode *root)
{
    int i;

    if (root == NULL)
        return;
    for (i = 0; i < 26; i++)
        freeTrie(root->character[i]);

    free(root);
}

// Frees a 2D Character array.
void free2DCharArray(char **array, int rows)
{
    int i;

    for (i = 0; i < rows; i++)
        free(array[i]);

    free(array);

    return;

}

// Frees a 2D Int array.
void free2DIntArray(int **array, int rows)
{
    int i;

    for (i = 0; i < rows; i++)
        free(array[i]);

    free(array);

    return;
}



