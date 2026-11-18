#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <log.h>
#include <tree_struct.h>

leaf_t* InitLeaf( const char* init_data );
leaf_t* ChangeLeafData( leaf_t* current, const char* new_data );
//leaf_t* InitTree( tree_t* tree, const char* init_data );
void DeleteLeaf( leaf_t* leaf );

void FPrintTree( leaf_t* leaf, FILE* file);
int SaveTree( leaf_t* leaf, const char* filename );
int DumpLeaf( leaf_t* leaf, size_t rank, FILE* file );
void GraphDumpTree( leaf_t* root );

leaf_t* AddLeft( leaf_t* current, const char* data );
leaf_t* AddRight( leaf_t* current, const char* data );
//leaf_t* GetRoot( tree_t* tree );
leaf_t* MoveLeft( leaf_t* current );
leaf_t* MoveRight( leaf_t* current );

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct file_data_t
{
    int file_stream;
    long file_size;
    ssize_t text_size;
    char* buffer;
    char filename[200];
};

void SetFileName( file_data_t* file, const char* filename );
char* PrepareBuffer( file_data_t* file );
void ReadFromFile( file_data_t* file );
leaf_t* LoadTree( const char* filename );
leaf_t* LoadLeaf( const char** buffer );

#endif
