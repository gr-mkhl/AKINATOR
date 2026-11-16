#include <main.h>

int main()
{
    STARTLOG()
    leaf_t* root = InitLeaf("ball");
    leaf_t* current = root;
    AddLeft(current, "doll");
    AddRight(current, "hall");
    current = MoveLeft(root);
    AddLeft(current, "wall");
    AddRight(current, "roll");
    current = MoveRight(root);
    AddLeft(current, "more");
    //AddRight(current, "floor");
    SaveTree(root, "hueta.txt");
    GraphDumpTree(root);
    DeleteLeaf(root);
    root = LoadTree("hueta.txt");
    FPrintTree(root, stdout);
    GraphDumpTree(root);
    DeleteLeaf(root);
    ENDLOG()
    return 0;
}


