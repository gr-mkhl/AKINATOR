#include <tree.h>

leaf_t* InitLeaf( const char* init_data )
{
    assert(init_data);

    leaf_t* new_leaf = (leaf_t*) calloc(1, sizeof(leaf_t));
    if (new_leaf == NULL)
        return NULL;

    new_leaf->data = strdup(init_data);

    return new_leaf;
}

leaf_t* ChangeLeafData( leaf_t* current, const char* new_data )
{
    free(current->data);
    current->data = strdup(new_data);

    return current;
}

leaf_t* AddLeft( leaf_t* current, const char* data )
{
    assert(data);

    if (current == NULL)
        return NULL;

    current->left = InitLeaf(data);
    return current->left;
}

leaf_t* AddRight( leaf_t* current, const char* data )
{
    assert(data);

    if (current == NULL)
        return NULL;

    current->right = InitLeaf(data);
    return current->right;
}
//TODO - что делать с current == NULL?
//это не тоже самое, что и current->left == NULL
//это ошибка
leaf_t* MoveLeft( leaf_t* current )
{
    assert(current);
    return current->left;
}
//TODO - что делать с current == NULL?
//это не тоже самое, что и current->right == NULL
//это ошибка
leaf_t* MoveRight( leaf_t* current )
{
    assert(current);
    return current->right;
}

void SaveTree( leaf_t* root, const char* filename )
{
    assert(filename);

    if (root == NULL)
    {
        fprintf(stderr, "Tree is empty, nothing to save\n");
        return;
    }

    FILE* output_file = fopen(filename, "w");
    assert(output_file);

    FPrintTree(root, output_file);

    fclose(output_file);
}

void FPrintTree( leaf_t* leaf, FILE* file )
{
    if (leaf == NULL)
    {
        fprintf(file, "(null)");
        return;
    }
    fprintf(file, "(%s", leaf->data);
    if (leaf->left != NULL || leaf->right != NULL)
    {
        FPrintTree(leaf->left, file);
        FPrintTree(leaf->right, file);
    }
    fprintf(file, ")");
}

void GraphDumpTree( leaf_t* root )
{
    if (root == NULL)
        return;

    char graph_file_name[70] = {};
    char time_buffer[20] = {};
    strcpy(time_buffer, FillTimeForNames());
    snprintf(graph_file_name, 70, "logs/txt/tree_graph_dot_code%s.txt", time_buffer);

    FILE* tree_graph = fopen(graph_file_name, "w");
    assert(tree_graph);

    fprintf(tree_graph, "digraph\n");
    fprintf(tree_graph, "{\n");
    fprintf(tree_graph, "rankdir=UD\n");

    DumpLeaf(root, 1, tree_graph);

    fprintf(tree_graph, "}");
    fclose(tree_graph);


    char command[150] = {};
    snprintf(command, 150, "dot -Tsvg %s -o logs/img/tree_graph_img%s.svg", graph_file_name, time_buffer);
    system(command);

    #ifdef LOGGING
    snprintf(command, 150, "<img src = ../img/tree_graph_img%s.svg>", time_buffer);
    Log(command);
    #endif
}

int DumpLeaf( leaf_t* leaf, size_t rank, FILE* file )
{
    if (leaf == NULL)
        return 0;
    fprintf(file,
    "{ rank = %zu leaf%zu [shape = Mrecord; style = filled; fillcolor = \"#00abbb\"; color = \"#006b75\";\n"
    "         label = \" { %s }\"]; }\n",
                rank, (size_t)leaf, leaf->data);

    if (DumpLeaf(leaf->left, rank + 1, file))
        fprintf(file, "leaf%zu -> leaf%zu [color = \"#008000\", weight = 100]\n", (size_t)leaf, (size_t)leaf->left);
    if (DumpLeaf(leaf->right, rank + 1, file))
        fprintf(file, "leaf%zu -> leaf%zu [color = \"#ff0000\", weight = 100]\n", (size_t)leaf, (size_t)leaf->right);
    return 1;
}

void DeleteLeaf( leaf_t* leaf )
{
    if (leaf->left != NULL)
        DeleteLeaf(leaf->left);
    if (leaf->right != NULL)
        DeleteLeaf(leaf->right);
    free(leaf->data);
    free(leaf);
}

leaf_t* LoadTree( const char* filename )
{
    assert(filename);

    file_data_t file_data = {};
    SetFileName(&file_data, filename);
    if (PrepareBuffer(&file_data) == NULL)
        return NULL;

    ReadFromFile(&file_data);

    leaf_t* root = NULL;

    if (*file_data.buffer == '(')
    {
        const char* tree_buffer = file_data.buffer + 1; //not include the first '('
        root = LoadLeaf(&tree_buffer);
    }

    free(file_data.buffer);

    return root;
}

leaf_t* LoadLeaf( const char** buffer )
{
    leaf_t* current = {};
    char temp[256] = {};

    if (sscanf(*buffer, "%255[^()]", temp) == EOF)
    {
        ON_LOG(LOG("Reached the end of the file"))
        return NULL;
    };

    *buffer += strlen(temp);

    if (strcmp(temp, "null") == 0)
    {
        (*buffer)++;
        return NULL;
    }

    current = InitLeaf(temp);

    if (**buffer == '(')
    {
        (*buffer)++;
        current->left = LoadLeaf(buffer);
    }

    if (**buffer == '(')
    {
        (*buffer)++;
        current->right = LoadLeaf(buffer);
    }

    if (**buffer == ')')
        (*buffer)++;


    return current;
}

// treeNode_t *loadNode(char *treeBuffer, treeNode_t *currentNode, treeNode_t *parent, size_t *index, char *string)
// {
//     if (sscanf(&treeBuffer[*index], "%256[^()]", string) == -1)
//     {
//         fprintf(stderr, "ERROR: incorrect reading from buffer\n");
//
//         return NULL;
//     }
//
//     *index += strlen(string);
//
//     if (strcmp(string, "nil") == 0)
//     {
//         (*index)++;
//
//         return NULL;
//     }
//
//     currentNode = createNode(parent, string);
//
//     if (treeBuffer[*index] == '(')
//     {
//         (*index)++;
//
//         currentNode->left = loadNode(treeBuffer, currentNode->left, currentNode, index, string);
//     }
//
//     if (treeBuffer[*index] == '(')
//     {
//         (*index)++;
//
//         currentNode->right = loadNode(treeBuffer, currentNode->right, currentNode, index, string);
//     }
//
//     if (treeBuffer[*index] == ')')
//     {
//         (*index)++;
//     }
//
//     return currentNode;
// }

void SetFileName( file_data_t* file, const char* filename )
{
    assert(file);
    assert(filename);

    char* set_file_status = strcpy(file->filename, filename);
    assert(set_file_status);

    return;
}

char* PrepareBuffer( file_data_t* file )
{
    assert(file);

    struct stat my_file = {};

    int stat_status = stat(file->filename, &my_file);
    if (stat_status == -1)
    {
        fprintf(stderr, "No such file or directory\n");
        return NULL;
    }

    file->file_size = my_file.st_size;

    file->buffer = (char*) calloc((size_t)(file->file_size + 1), sizeof(char));
    assert(file->buffer != NULL);

    file->buffer[file->file_size] = '\0';

    return file->buffer;
}

void ReadFromFile( file_data_t* file )
{
    assert(file);

    file->file_stream = open(file->filename, O_RDONLY);
    assert(file->file_stream != -1);

    file->text_size = read(file->file_stream, file->buffer, (size_t)file->file_size);
    assert(file->text_size > 0);

    int close_status = close(file->file_stream);
    assert(close_status != -1);

    file->buffer[file->text_size] = '\0';

    return;
}
