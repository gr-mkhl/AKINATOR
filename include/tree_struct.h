#ifndef _TREE_STRUCT_H_
#define _TREE_STRUST_H_

typedef char* data_t;
#define os "s"

// enum tree_err
// {
//     ALL_CORRECT = 0,
//     TREE_OOM    = 1  //достижимо ли?
// }
struct leaf_t
{
    leaf_t* left;
    leaf_t* right;
    data_t data;
};

// struct tree_t
// {
//     leaf_t* root;
//     leaf_t* current;
// };

#endif
