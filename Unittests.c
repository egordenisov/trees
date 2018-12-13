#ifndef UNITTESTS
#define UNITTESTS


int i = 0;

Node *CreateTree (int depth);

int Unittests (void)
{
	Node *tree1 = CreateTree (5);
/*
	i = 8;
	tree1 = CreateNode (&i, sizeof (int));
	i = 12;
	AddLeftNode (tree1, &i, sizeof (int));
	i = 15;
	AddRightNode (tree1, &i, sizeof (int));
	i = 13;
	AddLeftNode (tree1->left, &i, sizeof (int));
	i = 145;
	AddRightNode (tree1->left, &i, sizeof (int));
	i = 23;
	AddLeftNode (tree1->right, &i, sizeof (int));
	i = 67;
	AddRightNode (tree1->right, &i, sizeof (int));
*/
	DrawTree (tree1);

	DeleteSubTree (&tree1);

	return 0;
}
	
Node *CreateTree (int depth)
{
	if (depth < 0)
		return NULL;
	Node *tree = CreateNode (&i, sizeof (int));
	i++;
	tree->left = CreateTree (depth - 1);
	tree->right = CreateTree (depth - 1);
	return tree;
}

#endif //UNITTESTS

