#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "packing.h"

//static void Test(Node *head);
static void Free(Node *head);

int main()
{
    FILE *fp;
    Node *head;
    printf( "Opening the file test.c in read mode\n" ) ;

    fp = fopen ( "1K.pr", "r" ) ; // opening an existing file
    head = Build_BTree(fp);
    //Test(head);
    fclose ( fp ) ;

    fp = fopen ( "output.po", "w" ) ;
    Output1(head, fp);
    fclose ( fp ) ;

    int h = 0;
    int w = 0;
    fp = fopen ( "output.dim", "w" ) ;
    Output2(head, fp, &w, &h);
    fclose ( fp ) ;

    fp = fopen ( "output.pck", "w" ) ;
    Output3(head, fp, 0, 0, 0, 0, '0', '0');
    fclose ( fp ) ;

    printf("\nClosing the file test.c\n") ;
    Free(head);
    return 0;
}

/*void Test(Node *head)
{
    if(!(strcmp(head->label, "H")) || !(strcmp(head->label, "V")))
    {
        printf("\nLabel is %s\n",head->label);
        Test(head->left);
        Test(head->right);
    }
    else
    {
        printf("\nLabel is %s\n",head->label);
        printf("Width is %d\n", head->wid);
        printf("Height is %d\n", head->hei);
        if(head->left == NULL)
            printf("Left is NULL\n");
        if(head->right == NULL)
            printf("right is NULL\n");
    }
    return;
}*/

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