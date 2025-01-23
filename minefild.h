#ifndef _minefild_h
#define _minefild_h

#include "steps.h"
typedef struct{
    int x,y;
    // 0 visible; 1 is cover; 2 is flagd
    int * cover;
    // <0;8> how many mines; 9 mines egzist
    int * mines;
    int score;
    int multi;
} minefild;

minefild * minefild_innit(int size_x, int size_y, int mines_amount, int multi);

minefild* minefild_for_file(FILE* sorse);

void minefild_free(minefild*);

int minefild_cord_to_ind(minefild*, int, int);

void minefild_print(minefild*);

int minefild_open(minefild*, int, int);

void minefild_sopen(minefild*, int, int);

void minefild_flag(minefild*, int, int);

void minefild_to_file(minefild*,CommandLog*, FILE*);

int minefild_check_board(minefild*);

#endif