#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"reroot.h"

static int Find_LR_Edge(Node *head, Node **first, Node **second, int *n);
static int Find_RL_Edge(Node *head, Node **first, Node **second, int *n);
static void BT_Pre(FILE *fp, Node *head);
static void Find_Reroot_Tree(FILE *fp, int dir, int n, Node *head);
static void Find_Dimension(Node *head, int *w, int *h);
static void Pre_Traversal2(FILE *fp, Node *head, int n, int dir, int wll, int hll, int wlr, int hlr, int wrl, int hrl, int wrr, int hrr);
static void Decide_WH(Node *head, int *w, int *h, int wl, int hl, int wr, int hr);
Node *Build_BTree(FILE *fp, Node* pred){
    Node *head = NULL;
    long size;
    int n;
    long max;
    char c;
    char temp[10];
    size = 0;
    max = ftell(fp) + 1;
    //printf("max is %ld\n", max);
    while(1){ // Find the start of the line
        c = fgetc(fp);
        if(c != '\n')
        {
            size++; // Find the size of a row
            if(size >= max){ // When the pointer of file get to the start of the file
                fseek(fp, -1, SEEK_CUR);
                break;
            } 
            else
                fseek(fp, -2, SEEK_CUR);
        }
        else
        {
            break;
        }
    }
    //printf("The size of the line is %ld\n", size);
    n = 0;
    while(1) // Find the Label
    {
        c = fgetc(fp);
        //printf("%c",c);
        if(c != '\n' && c!= '(' && c != EOF)
        {
            n++; // Find the size of label
        }
        else
        {
            break;
        }
    }
    //printf("\n");
    //printf("Size of label is %d\n",n);
    fseek(fp, -(n + 1), SEEK_CUR); // char is 1 byte. As a result, label byte = n
    head = (Node *)malloc(sizeof(Node));
    if(head == NULL)
        return head;
    head->predecessor = pred;
    head->label = (char *)malloc(n+1);
    fread(head->label,n,1,fp);
    head->label[n] = '\0';
    //printf("Label is %s\n",head->label);
    if(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V")))
    {
        head->wid = 0;
        head->hei = 0;
        if(size != max)
            fseek(fp, -size-2, SEEK_CUR);
        head->right = Build_BTree(fp, head);
        head->left = Build_BTree(fp, head);
    }
    else
    {
        fseek(fp, 1, SEEK_CUR);
        n = 0;
        while(1) // Find the width
        {
            c = fgetc(fp);
            if(c!= ',')
            {
                n++; // Find the size of width
            }
            else
            {
                break;
            }
        }
        fseek(fp, -(n + 1), SEEK_CUR);
        fread(temp,n,1,fp);
        temp[n] = '\0';
        //printf("Width is %s\n",temp);
        head->wid = atoi(temp);
        fseek(fp, 1, SEEK_CUR);
        n = 0;
        while(1) // Find the height
        {
            c = fgetc(fp);
            if(c!= ')')
            {
                n++; // Find the size of height
            }
            else
            {
                break;
            }
        }
        fseek(fp, -(n + 1), SEEK_CUR);
        fread(temp,n,1,fp);
        temp[n] = '\0';
        //printf("Height is %s\n", temp);
        head->hei = atoi(temp);
        fseek(fp, 1, SEEK_CUR);
        head->left = NULL;
        head->right = NULL;
        if(size != max)
            fseek(fp, -size-2, SEEK_CUR);
    }
    return head;
}

void Output1(Node *head, FILE *fp){
    Node *first;
    Node *second;
    int dir;
    int n;
    dir = Find_LR_Edge(head, &first, &second, &n); // 0: second is first left child  1: second is first right child
    //printf("n = %d\n", n); 
    if((first == NULL) && (second == NULL)){
        BT_Pre(fp, head);
    }
    else{
        if(dir == 0){
            fprintf(fp,"%s\n",first->label);
            BT_Pre(fp, first->left);
            Find_Reroot_Tree(fp, 1, n-1, first->predecessor);
        }
        else{
            fprintf(fp,"%s\n",first->label);
            Find_Reroot_Tree(fp, 0, n-1, first->predecessor);
            BT_Pre(fp, first->right);
        }
    }
}

void Output2(Node *head, FILE *fp){
    Node *first;
    Node *second;
    int dir;
    int n;
    dir = Find_RL_Edge(head, &first, &second, &n); // 0: second is first left child  1: second is first right child
    //printf("n = %d\n", n); 
    if((first == NULL) && (second == NULL)){
        BT_Pre(fp, head);
    }
    else{
        if(dir == 0){
            fprintf(fp,"%s\n",first->label);
            BT_Pre(fp, first->left);
            Find_Reroot_Tree(fp, 1, n-1, first->predecessor);
        }
        else{
            fprintf(fp,"%s\n",first->label);
            Find_Reroot_Tree(fp, 0, n-1, first->predecessor);
            BT_Pre(fp, first->right);
        }
    }
}

void Output3(Node *head, FILE *fp){
    Pre_Traversal2(fp, head, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0);
}

int Find_LR_Edge(Node *head, Node **first, Node **second, int *n){
    int dir = 1;
    *n = 0;
    Node dummy;
    dummy.right = head;
    *first = &dummy;
    *second = head;
    while(1){
        if((*second)->left == NULL)
            break;
        else{
            *first = (*first)->right;
            *second = (*second)->left;
            *n = *n + 1;
            dir = 0;
        }
        if((*second)->right == NULL)
            break;
        else{
            *first = (*first)->left;
            *second = (*second)->right;
            *n = *n + 1;
            dir = 1;
        }
    }
    if(*n <=1){
        *first = NULL;
        *second = NULL;
    }
    return dir;
}

int Find_RL_Edge(Node *head, Node **first, Node **second, int *n){
    int dir = 1;
    *n = 0;
    Node dummy;
    dummy.left = head;
    *first = &dummy;
    *second = head;
    while(1){
        if((*second)->right == NULL)
            break;
        else{
            *first = (*first)->left;
            *second = (*second)->right;
            *n = *n + 1;
            dir = 1;
        }
        if((*second)->left == NULL)
            break;
        else{
            *first = (*first)->right;
            *second = (*second)->left;
            *n = *n + 1;
            dir = 0;
        }
    }
    if(*n <=1){
        *first = NULL;
        *second = NULL;
    }
    return dir;
}

void BT_Pre(FILE *fp, Node *head){
    while(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V"))){
        fprintf(fp,"%s\n",head->label);
        BT_Pre(fp, head->left);
        head = head->right;
    }
    fprintf(fp,"%s(%d,%d)\n", head->label, head->wid, head->hei);
}

void Find_Reroot_Tree(FILE *fp, int dir, int n, Node *head){
    fprintf(fp,"%s\n",head->label);
    if(n > 1){
        if(dir == 0){
            BT_Pre(fp, (head->left)->left);
            Find_Reroot_Tree(fp, 1, n-1, head->predecessor);
        }
        else{
            Find_Reroot_Tree(fp, 0, n-1, head->predecessor);
            BT_Pre(fp, (head->right)->right);
        }
    }
    else{
        if(dir == 0){
            BT_Pre(fp, (head->left)->left);
            BT_Pre(fp, head->right);
        }
        else{
            BT_Pre(fp, head->left);
            BT_Pre(fp, (head->right)->right);
        }
    }
}

void Find_Dimension(Node *head, int *w, int *h){
    if(head == NULL){
        *w = 0;
        *h = 0;
        return;
    }
    if(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V")))
    {
        int wl = 0;
        int hl = 0;
        int wr = 0;
        int hr = 0;
        Find_Dimension(head->left, &wl, &hl);
        Find_Dimension(head->right, &wr, &hr);
        if(!(strcmp(head->label,"H")))
        {
            if(wl > wr)
                *w = wl;
            else
                *w = wr;
            *h = hl + hr;
        }
        else
        {
            *w = wl + wr;
            if(hl > hr)
                *h = hl;
            else
                *h = hr;
        }
    }
    else
    {
        *w = head->wid;
        *h = head->hei;
    }
}

void Pre_Traversal2(FILE *fp, Node *head, int n, int dir, int wll, int hll, int wlr, int hlr, int wrl, int hrl, int wrr, int hrr){
    if(n <= 1){
        fprintf(fp,"%s\n",head->label);
        if(n == 0){
            if(head->left != NULL){
                Find_Dimension((head->left)->left, &wll, &hll);
                Find_Dimension((head->left)->right, &wlr, &hlr);
            }
            if(head-> right != NULL){
                Find_Dimension((head->right)->left, &wrl, &hrl);
                Find_Dimension((head->right)->right, &wrr, &hrr);
            }
        }
        if(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V"))){
            Pre_Traversal2(fp, head->left, n + 1, 0, wll, hll, wlr, hlr, wrl, hrl, wrr, hrr);
            Pre_Traversal2(fp, head->right, n + 1, 1, wll, hll, wlr, hlr, wrl, hrl, wrr, hrr);
        }
    }
    else{
        int w;
        int h;
        int wl;
        int hl;
        int wr;
        int hr;
        if(dir == 0){
            if(((head->predecessor)->predecessor)->left == head->predecessor){
                wl = wll;
                hl = hll;
                if(n == 2)
                    Decide_WH(((head->predecessor)->predecessor)->right, &wrr, &hrr, wrl, hrl, wrr, hrr);
                else
                    Decide_WH(((head->predecessor)->predecessor)->predecessor, &wrr, &hrr, wrl, hrl, wrr, hrr);
                wrl = wlr;
                hrl = hlr;
                Find_Dimension(head->left, &wll, &hll);
                Find_Dimension(head->right, &wlr, &hlr);
                Decide_WH((head->predecessor)->predecessor, &wr, &hr, wrl, hrl, wrr, hrr);
            }
            else{
                wl = wrl;
                hl = hrl;
                if(n == 2)
                    Decide_WH(((head->predecessor)->predecessor)->left, &wrl, &hrl, wll, hll, wlr, hlr);
                else
                    Decide_WH(((head->predecessor)->predecessor)->predecessor, &wrl, &hrl, wll, hll, wlr, hlr);
                Find_Dimension(head->left, &wll, &hll);
                Find_Dimension(head->right, &wlr, &hlr);
                Decide_WH((head->predecessor)->predecessor, &wr, &hr, wrl, hrl, wrr, hrr);
            }
        }
        else{
            if(((head->predecessor)->predecessor)->left == head->predecessor){
                wr = wlr;
                hr = hlr;
                if(n == 2)
                    Decide_WH(((head->predecessor)->predecessor)->right, &wlr, &hlr, wrl, hrl, wrr, hrr);
                else
                    Decide_WH(((head->predecessor)->predecessor)->predecessor, &wlr, &hlr, wrl, hrl, wrr, hrr);
                Find_Dimension(head->left, &wrl, &hrl);
                Find_Dimension(head->right, &wrr, &hrr);
                Decide_WH((head->predecessor)->predecessor, &wl, &hl, wll, hll, wlr, hlr);
            }
            else{
                wr = wrr;
                hr = hrr;
                if(n == 2)
                    Decide_WH(((head->predecessor)->predecessor)->left, &wll, &hll, wll, hll, wlr, hlr);
                else
                    Decide_WH(((head->predecessor)->predecessor)->predecessor, &wll, &hll, wll, hll, wlr, hlr);
                wlr = wrl;
                hlr = hrl;
                Find_Dimension(head->left, &wrl, &hrl);
                Find_Dimension(head->right, &wrr, &hrr);
                Decide_WH((head->predecessor)->predecessor, &wl, &hl, wll, hll, wlr, hlr);
            }
        }
        Decide_WH(head->predecessor, &w, &h, wl, hl, wr, hr);
        fprintf(fp,"%s(%d,%d)\n",head->label, w, h);
        //printf("Label is %s, (%d %d), (%d %d), (%d %d), (%d %d) \n",head->label, wll, hll, wlr, hlr, wrl, hrl, wrr, hrr);
        if(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V"))){
            Pre_Traversal2(fp, head->left, n + 1, 0, wll, hll, wlr, hlr, wrl, hrl, wrr, hrr);
            Pre_Traversal2(fp, head->right, n + 1, 1, wll, hll, wlr, hlr, wrl, hrl, wrr, hrr);
        }
    }
}

void Decide_WH(Node *head, int *w, int *h, int wl, int hl, int wr, int hr){
    if(!(strcmp(head->label,"H")))
        {
            if(wl > wr)
                *w = wl;
            else
                *w = wr;
            *h = hl + hr;
        }
        else if(!(strcmp(head->label,"V")))
        {
            *w = wl + wr;
            if(hl > hr)
                *h = hl;
            else
                *h = hr;
        }
        else{
            *w = head->wid;
            *h = head->hei;
        }
}