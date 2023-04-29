#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "packing.h"

static void Free(Node *head);

int main(int argc, char *argv[])
{
    FILE *fp;
    Node *head;

    fp = fopen ( argv[1], "r" ) ; // opening an existing file
    if(fp == NULL)
        return EXIT_FAILURE;
    head = Build_BTree(fp);
    fclose ( fp ) ;
    if(head == NULL)
        return EXIT_FAILURE;

    fp = fopen ( argv[2], "w" ) ;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output1(head, fp);
    fclose ( fp ) ;

    int h = 0;
    int w = 0;
    fp = fopen ( argv[3], "w" ) ;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output2(head, fp, &w, &h);
    fclose ( fp ) ;

    fp = fopen ( argv[4], "w" ) ;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output3(head, fp, 0, 0, 0, 0, '0', '0');
    fclose (fp) ;

    Free(head);
    return EXIT_SUCCESS;
}

void Free(Node *head)
{
    if(!(strcmp(head->label, "H")) || !(strcmp(head->label, "V")))
    {
        Free(head->left);
        Free(head->right);
        free(head->label);
        free(head);
    }
    else
    {
        free(head->label);
        free(head);
    }
    return;
}