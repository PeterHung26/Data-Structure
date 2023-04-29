#include<stdio.h>
#include<stdlib.h>
#include"evaluation.h"
#include"hbt.h"

static int Build_BST(FILE *fp, Tnode **head, int *height);

int Read_File_Eval(FILE *fp, Tnode **head){
    int read = 0;
    int height = 0;
    Tnode *temp = NULL;
    read = Build_BST(fp, &temp, &height);
    *head = temp;
    return read;
}

int Build_BST(FILE *fp, Tnode **head, int *height){
    int data;
    char child;
    int valid = 0;
    int left = 0;
    int right = 0;
    if(!fread(&data, 4, 1, fp)){
        return valid;
    }
    if((data > HBT_MAX)|| (data < HBT_MIN)){
        return valid;
    }
    *head = (Tnode *)malloc(sizeof(Tnode));
    if((*head) == NULL){
        return valid;
    }
    (*head)->key = data;
    if(!fread(&child, 1, 1, fp)){
        return valid;
    }
    switch(child){
        case 0:
            (*head)->left = NULL;
            (*head)->right = NULL;
            *height = 1;
            (*head)->balance = 0;
            valid = 1;
            break;
        case 1:
            (*head)->left = NULL;
            valid = Build_BST(fp, &((*head)->right), &right);
            if(valid == 0)
                break;
            *height = right + 1;
            if(right > 4)
                (*head)->balance = -4;
            else
                (*head)->balance = -right;
            break;
        case 2:
            valid = Build_BST(fp, &((*head)->left), &left);
            (*head)->right = NULL;
            if(valid == 0)
                break;
            *height = left + 1;
            if(left > 3)
                (*head)->balance = 3;
            else
                (*head)->balance = left;
            break;
        case 3:
            valid = Build_BST(fp, &((*head)->left), &left);
            if(valid == 0)
                break;
            valid = Build_BST(fp, &((*head)->right), &right);
            if(valid == 0)
                break;
            if(((left - right) > 3) || ((left - right) < -4)){
                if((left - right) > 3)
                    (*head)->balance = 3;
                else
                    (*head)->balance = -4;
            }
            else
                (*head)->balance = (left - right);
            if(left > right)
                *height = left + 1;
            else
                *height = right + 1;
            break;
        default:
            valid = 0;
            break;
    }
    return valid;
}

int BST_Eval(Tnode *head){
    int bst = 0;
    if((head->left == NULL) && (head->right == NULL)){
        bst = 1;
        return bst;
    }
    if(head->left != NULL){
        if(((head->left)->key) <= head->key)
            bst = 1;
        if(bst == 0)
            return bst;
    }
    if(head->right != NULL){
        if(((head->right)->key) >= head->key)
            bst = 1;
        if(bst == 0)
            return bst;
    }
    if(head->left != NULL){
        bst = BST_Eval(head->left);
        if(bst == 0)
            return bst;
    }
    if(head->right != NULL){
        bst = BST_Eval(head->right);
        if(bst == 0)
            return bst;
    }
    return bst;
}

int HBT_Eval(Tnode *head){
    int hbtt = 0;
    if((head->balance > 1) || (head->balance < -1)){
        hbtt = 0;
        return hbtt;
    }
    else
        hbtt = 1;
    if(head->left != NULL){
        hbtt = HBT_Eval(head->left);
        if(hbtt == 0)
            return hbtt;
    }
    if(head->right != NULL){
        hbtt = HBT_Eval(head->right);
        if(hbtt == 0)
            return hbtt;
    }
    return hbtt;
}