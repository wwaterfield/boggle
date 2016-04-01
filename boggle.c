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
int isPrefix(TrieNode *root, char *str);
int **visitedArray(void);
void findWords(TrieNode *root, int **visitedArray, char **board, int curX, int curY, int k, char *string);
int isWord(TrieNode *root, char *str);

int main(void)
{
    // Declaring variables.
    int numWords, numCases, i, len, **visited;
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
        if (len >= STR_MIN && len <= STR_MAX)
            root = insertString(root, temp);
    }

    // Process each game individually.
    scanf("%d", &numCases);

    for (i = 0; i < numCases; i++)
    {
        char string[17];
        string[0] = '\0';
        // Read in each game board.
        board = readBoard();

		// Generate a visited array.
		visited = visitedArray();

        // Debug print board.
        print2DArray(board);

		// Finds words in the boggle puzzle and prints them out.
		findWords(root, visited, board, 0, 0, 0, string);
    }



	return 0;
}

void findWords(TrieNode *root, int **visitedArray, char **board, int curX, int curY, int k, char *string)
{
	int i, rows, cols, newX, newY;

	visitedArray[curX][curY] = 1;
	string[k] = board[curX][curY];
	//printf("Made it #1\n");

    for (rows = 0; rows < BOARD_SIZE; rows++)
    {
        for (cols = 0; cols < BOARD_SIZE; cols++)
        {
            for (i = 0; i < DX_SIZE; i++)
            {
                newX = rows + DX[i];
                newY = rows + DY[i];

                if (inBounds(newX, newY) == 0)
                    continue;
                /*
                printf("Made it #2\n");
                printf("NewX: %d NewY: %d\n", newX, newY);
                printf("New Letter: %c", board[newX][newY]);
                printf("%s\n", string);
                */
                string[k+1] = board[newX][newY];
                string[k+2] = '\0';

                //printf("Made it #3\n");

                if (isPrefix(root, string))
                {
                    //printf("Made it #4");
                    if(isWord(root, string))
                        printf("%s\n", string);
                    findWords(root, visitedArray, board, newX, newY, k+1, string);
                }
                else return;
            }
        }
	}

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

// Inserts a given string into the trie structure.
TrieNode *insertString(TrieNode *root, char *str)
{
    int i, index;
    int length = (int)strlen(str);
    TrieNode *temp = root;

    if (root == NULL)
        root = temp = createTrieNode();

    for (i = 0; i < length; i++)
    {
        index = tolower(str[i]) - 'a';

        if (temp->character[index] == NULL)
            temp->character[index] = createTrieNode();

        temp = temp->character[index];
    }

    temp->isWord = 1;

    return root;
}

// Determines if a given string is a prefix in the trie.
int isPrefix(TrieNode *root, char *str)
{
	int i, index;
	int length = strlen(str);
	TrieNode *temp = root;

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
	for (i = 0; i < length; i++)
	{
		index = tolower(str[i]) - 'a';
		//printf("Indx: %d\n", index);


		if(temp->character[index] == NULL)
		{
            //printf("Not Found: %c\n", index + 'a');
			return 0;
        }
		temp = temp->character[index];
	}
	if (temp->isWord == 1)
		return 1;
}

// Frees the memory associated with the trie.
void freeTrie(TrieNode *root)
{
    int i;
    if (root == NULL)
        return;

    for (i = 0; i < 26; i++)
        freeTrie(root->character[i]);
    free(root);
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

// Determines if a given set of coordinates is out of boundss
int inBounds(int newX, int newY)
{
	if (newX > BOARD_SIZE - 1 || newX < 0 || newY > BOARD_SIZE -1 || newY < 0)
		return 0;
	else return 1;
}

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



