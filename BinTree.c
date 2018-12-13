#ifndef BINARY_TREE
#define BINARY_TREE


Node *CreateNode (const BIN_TREE_DATA_TYPE *value, int size)
{
	ASSERT (value);
	ASSERT (size >= 0);
	
	Node *root = (Node *) calloc (1, sizeof (*root));
	if (!root)
	{
		fprintf (stderr, "CreateNode: can't allocate memory\n");
		return NULL;
	}

	root->left = NULL;
	root->right = NULL;
	root->data = (BIN_TREE_DATA_TYPE *) calloc (size, sizeof (*(root->data)));
	if (!(root->data))
	{
		fprintf (stderr, "CreateNode: can't allocate memory\n");
		return NULL;
	}

	memcpy (root->data, value, size);
	
	return root;
}

int AddLeftNode (Node *root, const BIN_TREE_DATA_TYPE *value, int size)
{
	ASSERT (root);
	ASSERT (value);
	ASSERT (size >= 0);

	if (root->left)
		DeleteSubTree (&(root->left));

	root->left = CreateNode (value, size);

	return 0;
}

int AddRightNode (Node *root, const BIN_TREE_DATA_TYPE *value, int size)
{
	ASSERT (root);
	ASSERT (value);
	ASSERT (size >= 0);

	if (root->right)
		DeleteSubTree (&(root->right));

	root->right = CreateNode (value, size);

	return 0;
}

int DeleteSubTree (Node **root)
{
	ASSERT (root);
	ASSERT (*root);

	if ((*root)->left)
		DeleteSubTree (&((*root)->left));
	if ((*root)->right)
		DeleteSubTree (&((*root)->right));

	if ((*root)->data)
		free ((*root)->data);
	free (*root);
	*root = NULL;
	return 0;
}

int DumpTreeInt (Node *root)
{
	ASSERT (root);

	printf ("Data: %d\n", *(root->data));
	if (root->left)
	{
		printf ("Left child:\n(\n");
		DumpTreeInt (root->left);
		printf (")\n");
	}
	if (root->right)
	{
		printf ("Right child:\n(\n");
		DumpTreeInt (root->right);
		printf (")\n");
	}
}

#endif //BINARY_TREE

