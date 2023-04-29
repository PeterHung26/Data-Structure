#ifndef __PACKING_H__
#define __PACKING_H__
#include<stdio.h>

typedef struct _Node {
    char *label;
    int wid;
    int hei;
    struct _Node *left;
    struct _Node *right;
} Node;

Node *Build_BTree(FILE *fp);

void Output1(Node *head, FILE *fp);

void Output2(Node *head, FILE *fp, int *w, int *h);

void Output3(Node *head, FILE *fp, int w, int h, int x, int y, char mode, char dir);
#endif