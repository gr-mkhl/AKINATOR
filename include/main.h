#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <tree.h>
#include <log.h>

void CleanBuffer();

#define MENU "menu:\n"                       \
                "1 - init tree\n"            \
                "2 - go to root\n"           \
                "3 - move to left child\n"   \
                "4 - move to right child\n"  \
                "5 - add left child\n"       \
                "6 - add right child\n"      \
                "p - print tree\n"           \
                "d - delete tree\n"          \
                "q - exit program\n"
#endif
