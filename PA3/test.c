#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"reroot.h"

//static void Test(Node *head);
static void Free(Node *head);

int main(){
    FILE *fp;
    Node *head;
    printf( "Opening the file test.c in read mode\n" ) ;
    fp = fopen ( "1M.po", "r" ) ; // opening an existing file
    if(fp == NULL)
        return EXIT_FAILURE;
    //fseek(fp, 0, SEEK_END);
    //long max = ftell(fp);
    //printf("The size of file is %ld byte\n",max);
    fseek(fp, -2, SEEK_END); 
    head = Build_BTree(fp, NULL);
    fclose (fp) ;

    fp = fopen ( "outputlr.pr", "w" ) ;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output1(head, fp);
    fclose (fp) ;

    fp = fopen ( "outputrl.pr", "w" ) ;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output2(head, fp);
    fclose (fp) ;

    /*fp = fopen ( "output.rdim", "w" ) ;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output3(head, fp);
    fclose (fp) ;*/

    fp = fopen ( "output2.rdim", "w" ) ;
    if(fp == NULL)
        return EXIT_FAILURE;
    Output4(head, fp);
    fclose (fp) ;

    //Test(head);
    Free(head);
}

/*void Test(Node *head)
{
    if(!(strcmp(head->label, "H")) || !(strcmp(head->label, "V")))
    {
        printf("\nLabel is %s\n",head->label);
        if((head->predecessor) == NULL)
            printf("Node has no predecessor\n");
        else
            printf("Predecessor is %s\n",(head->predecessor)->label);
        Test(head->left);
        Test(head->right);
    }
    else
    {
        printf("\nLabel is %s\n",head->label);
        printf("Width is %d\n", head->wid);
        printf("Height is %d\n", head->hei);
        if((head->predecessor) == NULL)
            printf("Node has no predecessor\n");
        else
            printf("Predecessor is %s\n",(head->predecessor)->label);
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