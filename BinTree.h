#ifndef HEADER
#define HEADER

#define BIN_TREE_DATA_TYPE int

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(CONDITION) 																	\
	if(!(CONDITION))																		\
	{																						\
		fprintf (stderr, "Assertation failed, file %s, function %s, line %d\n",				\
				__FILE__, __PRETTY_FUNCTION__, __LINE__);									\
		exit (EXIT_SUCCESS);																\
	}


typedef struct TreeNode
{
	struct TreeNode *left;
	struct TreeNode *right;
	BIN_TREE_DATA_TYPE *data;
} Node;

Node *CreateNode (const BIN_TREE_DATA_TYPE *value, int size);
int AddLeftNode (Node *root, const BIN_TREE_DATA_TYPE *value, int size);
int AddRightNode (Node *root, const BIN_TREE_DATA_TYPE *value, int size);
int DeleteSubTree (Node **root);
int DumpTreeInt (Node *root);

int Unittests (void);

int DrawTree (Node *root);

int SaveTree (Node *root);

#include "BinTree.c"
#include "Unittests.c"
#include "DrawTree.c"

#endif //HEADER

