#ifndef __LOADING__
#define __LOADING__
#include"vhnode.h"
#include<stdio.h>

void ROW_AND_COL(FILE *fp, int *M, int *N);

void RESET_VALUE(Node *vboard, int vrow, int vcol, Node *hboard, int hrow, int hcol, PQ *vheap, PQ *hheap);

int SET_BOARD(FILE *fp, Node *vboard, int vrow, int vcol);

void CAL_TOTAL_SHORT(Node *vboard, int vrow, int vcol, Node *hboard, int hrow, int hcol, PQ *vheap, PQ *hheap);

int SEARCH_SHORTEST(Node *hboard, int hrow, int hcol);

void PRINT_PATH(FILE *fp, int rrow, int ccol, char typpe, Node *vboard, int vrow, int vcol, Node *hboard, int hrow, int hcol);
#endif