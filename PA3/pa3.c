#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"reroot.h"

static void Free(Node *head);

int main(int argc, char *argv[]){
    FILE *fp;
    Node *head;
    if(argv[1] != NULL)
        fp = fopen ( argv[1], "r" ) ; // opening an existing file
    else
        return EXIT_FAILURE;
    if(fp == NULL)
        return EXIT_FAILURE;
    fseek(fp, -2, SEEK_END); 
    head = Build_BTree(fp, NULL);
    if(head == NULL)
        return EXIT_FAILURE;
    fclose (fp) ;

    if(argv[2] != NULL)
        fp = fopen ( argv[2], "w" ) ;
    else
        return EXIT_FAILURE;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output1(head, fp);
    fclose (fp) ;

    if(argv[3] != NULL)
        fp = fopen ( argv[3], "w" ) ;
    else
        return EXIT_FAILURE;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output2(head, fp);
    fclose (fp) ;

    if(argv[4] != NULL)
        fp = fopen ( argv[4], "w" ) ;
    else
        return EXIT_FAILURE;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output3(head, fp);
    fclose (fp) ;

    Free(head);
    if(argv[5] != NULL)
        return EXIT_FAILURE;
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