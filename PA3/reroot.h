#ifndef __REROOT_H__
#define __REROOT_H__
#include<stdio.h>

typedef struct _Node {
    char *label;
    int wid;
    int hei;
    struct _Node *left;
    struct _Node *right;
    struct _Node *predecessor;
} Node;

Node *Build_BTree(FILE *fp, Node* pred);

void Output1(Node *head, FILE *fp);

void Output2(Node *head, FILE *fp);

void Output3(Node *head, FILE *fp);
#endif