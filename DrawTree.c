#ifndef DRAW_DOT
#define DRAW_DOT

#define FILE_DOT_NAME "TreeDot.txt"
#define DRAW_DOT_NAME "Tree.jpg"


void PrintNodeInt (Node *root, FILE *stream);

int DrawTree (Node* root)
{
    FILE *stream = fopen (FILE_DOT_NAME, "w+");
    
    fputs ("Digraph G\n{\n", stream);
    
    PrintNodeInt (root, stream);

    fputs ("}\n", stream);

    fclose (stream);

    system ("dot " FILE_DOT_NAME " -Tjpg > " DRAW_DOT_NAME);
//    system ("rm FILE_DOT_NAME");
}

void PrintNodeInt (Node *root, FILE *stream)
{
    if (root->left)
    {
        fprintf (stream, "\t%d -> %d;\n", *(root->data), *(root->left->data));

        PrintNodeInt (root->left, stream);
    }

    if (root->right)
    {
        fprintf (stream, "\t%d -> %d;\n", *(root->data), *(root->right->data));

        PrintNodeInt (root->right, stream);
    }
    
}

#endif //DRAW_DOT

