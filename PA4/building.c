#include<stdio.h>
#include<stdlib.h>
#include"building.h"
#include"hbt.h"

static Tnode *CCR(Tnode *old);
static Tnode *CR(Tnode *old);
static Tnode *Create(int keyy, Tnode *left, Tnode *right);
static void  Insertion_Search(Tnode *head, int keyy, Tnode **curr, Tnode **pcurr, Tnode **ya, Tnode **pya);
static Tnode *Insertion_Balance(int keyy, Tnode *ya, Tnode *q);
static Tnode *Rotation(Tnode *ya, Tnode *child);
static Tnode *Deletion_Search(Tnode *curr, Tnode *pcurr, int keyy);
static int Two_Children(Tnode **curr, Tnode **pcurr);

Tnode *Insertion(Tnode *head, int keyy, int* error){
    Tnode *curr;
    Tnode *pcurr;
    Tnode *ya;
    Tnode *pya;
    Tnode *q;
    *error = 1;
    Insertion_Search(head, keyy, &curr, &pcurr, &ya, &pya);
    q = Create(keyy, NULL, NULL);
    if(q == NULL){
        *error = 0;
        return head;
    }
    q->balance = 0;
    if(pcurr == NULL)
        head = q;
    else{
        if(keyy <= (pcurr->key)){
            pcurr->left = q;
        }
        else{
            pcurr->right = q;
        }
    }
    if(pya == NULL){
        if(ya == NULL)
            ya = head;
        head = Insertion_Balance(keyy, ya, q);
    }
    else{
        if(ya == pya->left)
            pya->left = Insertion_Balance(keyy, ya, q);
        else
            pya->right = Insertion_Balance(keyy, ya, q);
    }
    return head;
}

Tnode *Deletion(Tnode *head, int keyy, int *error){
    *error = 1;
    if(head == NULL)
        return head;
    head = Deletion_Search(head, NULL, keyy);
    return head;
}

Tnode *CCR(Tnode *old){
    if(old->right != NULL){
        Tnode *new = old->right;
        old->right = new->left;
        new->left = old;
        return new;
    }
    else
        return old;
}

Tnode *CR(Tnode *old){
    if(old->left != NULL){
        Tnode *new = old->left;
        old->left = new->right;
        new->right = old;
        return new;
    }
    else
        return old;
}

Tnode *Create(int keyy, Tnode *left, Tnode *right){
    Tnode *temp = NULL;
    temp = (Tnode *)malloc(sizeof(Tnode));
    if(temp == NULL)
        return NULL;
    temp->key = keyy;
    temp->left = left;
    temp->right = right;
    return temp;
}

void  Insertion_Search(Tnode *head, int keyy, Tnode **curr, Tnode **pcurr, Tnode **ya, Tnode **pya){
    Tnode *q;
    *pcurr = NULL;
    *curr  = head;
    *ya = *curr;
    *pya = *pcurr;
    while(*curr != NULL){
        if(keyy <= (*curr)->key){
            q = (*curr)->left;
        }
        else{
            q = (*curr)->right;
        }
        if((q != NULL) && ((q->balance) != 0)){
            *pya = *curr;
            *ya = q;
        }
        *pcurr = *curr;
        *curr = q;
    }
}

Tnode *Insertion_Balance(int keyy, Tnode *ya, Tnode *q){
    Tnode *curr;
    Tnode *child;
    curr = ya;
    while(curr != q){
        if((q->key) <= curr->key){
            curr->balance = curr->balance + 1;
            curr = curr->left;
        }
        else{
            curr->balance = curr->balance - 1;
            curr = curr->right;
        }
    }
    if(((ya->balance) < 2) && ((ya->balance) > -2))
        return ya;
    else{
        if(keyy <= (ya->key))
            child = ya->left;
        else
            child = ya->right;
    }
    ya = Rotation(ya, child);
    return ya;
}

Tnode *Rotation(Tnode *ya, Tnode *child){
    Tnode *curr;
    if(((ya->balance) == 2) && ((child->balance) == 1)){
        curr = CR(ya);
        ya->balance = 0;
        child->balance = 0;
    }
    else if(((ya->balance) == 2) && ((child->balance) == 0)){
        curr = CR(ya);
        ya->balance = 1;
        child->balance = -1;
    }
    else if(((ya->balance) == -2) && ((child->balance) == -1)){
        curr = CCR(ya);
        ya->balance = 0;
        child->balance = 0;
    }
    else if(((ya->balance) == -2) && ((child->balance) == 0)){
        curr = CCR(ya);
        ya->balance = -1;
        child->balance = 1;
    }
    else if(((ya->balance) == 2) && ((child->balance) == -1)){
        ya->left = CCR(child);
        curr = CR(ya);
        if(curr->balance == 0){ // curr is inserted node q
            ya->balance = 0;
            child->balance = 0;
        }
        else{
            if(curr->balance == 1){
                ya->balance = -1;
                child->balance = 0;
            }
            else{
                ya->balance = 0;
                child->balance = 1;
            }
            curr->balance = 0;
        }
    }
    else if(((ya->balance) == -2) && ((child->balance) == 1)){
        ya->right = CR(child);
        curr = CCR(ya);
        if(curr->balance == 0){ // curr is inserted node q
            ya->balance = 0;
            child->balance = 0;
        }
        else{
            if(curr->balance == -1){
                ya->balance = 1;
                child->balance = 0;
            }
            else{
                ya->balance = 0;
                child->balance = -1;
            }
            curr->balance = 0;
        }
    }
    return curr;
}

Tnode *Deletion_Search(Tnode *curr, Tnode *pcurr, int keyy){
    int original;
    if(keyy < curr->key){
        if((curr->left) != NULL){
            original = curr->balance;
            curr->left = Deletion_Search(curr->left, curr, keyy);
            if(pcurr == NULL){
                if((curr->balance == 2) || (curr->balance == -2)){
                    if(curr->balance == 2){
                        curr = Rotation(curr, curr->left);
                    }
                    else{
                        curr = Rotation(curr, curr->right);
                    }
                    return curr;
                }
                else{
                    return curr;
                }
            }
            else{
                if(original == 0){
                    return curr;
                }
                else{
                    if((curr->balance == 1) || (curr->balance == -1)){
                        return curr;
                    }
                    else if(curr->balance == 0){
                        if(curr == pcurr->left)
                            pcurr->balance = pcurr->balance - 1;
                        else
                            pcurr->balance = pcurr->balance + 1;
                        return curr;
                    }
                    else{
                        if(curr->balance == 2)
                            curr = Rotation(curr, curr->left);
                        else
                            curr = Rotation(curr, curr->right);
                        if((curr->balance == 1) || (curr->balance == -1)){
                            return curr;
                        }
                        else if(curr->balance == 0){
                            if(curr == pcurr->left)
                                pcurr->balance = pcurr->balance - 1;
                            else
                                pcurr->balance = pcurr->balance + 1;
                            return curr;
                        }
                    }
                }
            }
        }
        else{
            return curr; // The deleted node is not in the tree
        }
    }
    else if(keyy > curr->key){
        if((curr->right) != NULL){
            original = curr->balance;
            curr->right = Deletion_Search(curr->right, curr, keyy);
            if(pcurr == NULL){
                if((curr->balance == 2) || (curr->balance == -2)){
                    if(curr->balance == 2){
                        curr = Rotation(curr, curr->left);
                    }
                    else{
                        curr = Rotation(curr, curr->right);
                    }
                    return curr;
                }
                else{
                    return curr;
                }
            }
            else{
                if(original == 0){
                    return curr;
                }
                else{
                    if((curr->balance == 1) || (curr->balance == -1)){
                        return curr;
                    }
                    else if(curr->balance == 0){
                        if(curr == pcurr->left)
                            pcurr->balance = pcurr->balance - 1;
                        else
                            pcurr->balance = pcurr->balance + 1;
                        return curr;
                    }
                    else{
                        if(curr->balance == 2)
                            curr = Rotation(curr, curr->left);
                        else
                            curr = Rotation(curr, curr->right);
                        if((curr->balance == 1) || (curr->balance == -1)){
                            return curr;
                        }
                        else if(curr->balance == 0){
                            if(curr == pcurr->left)
                                pcurr->balance = pcurr->balance - 1;
                            else
                                pcurr->balance = pcurr->balance + 1;
                            return curr;
                        }
                    }
                }
            }
        }
        else{
            return curr; // The deleted node is not in the tree
        }
    }
    else{ // Curr is the deleted node
        if(((curr->left) == NULL) && ((curr->right) == NULL)){
            free(curr);
            if(pcurr == NULL) // If the deleted node is the only node in the tree
                return NULL;
            else if((pcurr->left) == curr)
                pcurr->balance = pcurr->balance - 1;
            else
                pcurr->balance = pcurr->balance + 1;
            return NULL;
        }
        else if(((curr->left) == NULL) || ((curr->right) == NULL)){
            if((curr->left) != NULL){
                curr->key = (curr->left)->key;
                curr->balance = 0;
                free(curr->left);
                curr->left = NULL;
            }
            else{
                curr->key = (curr->right)->key;
                curr->balance = 0;
                free(curr->right);
                curr->right = NULL;
            }
            if(pcurr == NULL)
                return curr;
            if((pcurr->left) == curr)
                pcurr->balance = pcurr->balance - 1;
            else
                pcurr->balance = pcurr->balance + 1;
            return curr;
        }
        else{
            if(((curr->left)->right) != NULL){
                original = (curr->left)->balance;
                int pkey = Two_Children(&((curr->left)->right), &(curr->left));
                curr->key = pkey;
                if(original == 0){ // Height of subtree doesn't change.
                    return curr;
                }
                else{
                    if(((curr->left)->balance == 1) || ((curr->left)->balance == -1)){ // Height of subtree doesn't change.
                        return curr;
                    }
                    else if(((curr->left)->balance == 0)){ // Height of subtree change short.
                        if(curr->balance == 0){
                            curr->balance = curr->balance - 1;
                            return curr;
                        }
                        else if (curr->balance == 1){
                            curr->balance = curr->balance - 1;
                            if(pcurr == NULL)
                                return curr;
                            if(curr == pcurr->left)
                                pcurr->balance = pcurr->balance - 1;
                            else
                                pcurr->balance = pcurr->balance + 1;
                            return curr;
                        }
                        else{
                            curr->balance = curr->balance - 1;
                            curr = Rotation(curr, curr->right);
                            if(((curr->balance) == -1) || ((curr->balance) == 1))
                                return curr;
                            else{
                                if(pcurr == NULL)
                                    return curr;
                                if(curr == pcurr->left)
                                    pcurr->balance = pcurr->balance - 1;
                                else
                                    pcurr->balance = pcurr->balance + 1;
                                return curr;
                            }
                        }
                    }
                    else{
                        if(((curr->left)->balance == 2)){
                            curr->left = Rotation(curr->left, (curr->left)->left);
                        }
                        else{
                            curr->left = Rotation(curr->left, (curr->left)->right);
                        }
                        if(((curr->left)->balance == 1) || ((curr->left)->balance == -1)){ // Height of subtree doesn't change.
                            return curr;
                        }
                        else{ // Height of subtree change short.
                            if(curr->balance == 0){
                                curr->balance = curr->balance - 1;
                                return curr;
                            }
                            else if (curr->balance == 1){
                                curr->balance = curr->balance - 1;
                                if(curr == pcurr->left)
                                    pcurr->balance = pcurr->balance - 1;
                                else
                                    pcurr->balance = pcurr->balance + 1;
                                return curr;
                            }
                            else{
                                curr->balance = curr->balance - 1;
                                curr = Rotation(curr, curr->right);
                                if(((curr->balance) == -1) || ((curr->balance) == 1))
                                    return curr;
                                else{
                                    if(curr == pcurr->left)
                                        pcurr->balance = pcurr->balance - 1;
                                    else
                                        pcurr->balance = pcurr->balance + 1;
                                    return curr;
                                }
                            }
                        }
                    }
                }
            }
            else{
                if(((curr->left)->left) != NULL){ // Predecessor has a left node
                    curr->key = (curr->left)->key;
                    (curr->left)->key = ((curr->left)->left)->key;
                    free((curr->left)->left);
                    (curr->left)->left = NULL;
                    (curr->left)->balance = 0;
                    if(curr->balance == 0){
                        curr->balance = curr->balance -1;
                        return curr;
                    }
                    else if(curr->balance == 1){
                        curr->balance = curr->balance -1;
                        if(pcurr == NULL)
                            return curr;
                        if(curr == pcurr->left)
                            pcurr->balance = pcurr->balance - 1;
                        else
                            pcurr->balance = pcurr->balance + 1;
                        return curr;
                    }
                    else{
                        curr->balance = curr->balance - 1;
                        curr = Rotation(curr, curr->right);
                        if(((curr->balance) == -1) || ((curr->balance) == 1))
                            return curr;
                        else{
                            if(pcurr == NULL)
                                return curr;
                            if(curr == pcurr->left)
                                pcurr->balance = pcurr->balance - 1;
                            else
                                pcurr->balance = pcurr->balance + 1;
                            return curr;
                        }
                    }
                }
                else{ // Predecessor is a leaf node
                    curr->key = (curr->left)->key;
                    free(curr->left);
                    curr->left = NULL;
                    if(curr->balance == 0){
                        curr->balance = curr->balance -1;
                        return curr;
                    }
                    else if(curr->balance == 1){
                        curr->balance = curr->balance -1;
                        if(pcurr == NULL)
                            return curr;
                        if(curr == pcurr->left)
                            pcurr->balance = pcurr->balance - 1;
                        else
                            pcurr->balance = pcurr->balance + 1;
                        return curr;
                    }
                    else{
                        curr->balance = curr->balance - 1;
                        curr = Rotation(curr, curr->right);
                        if(((curr->balance) == -1) || ((curr->balance) == 1))
                            return curr;
                        else{
                            if(pcurr == NULL)
                                return curr;
                            if(curr == pcurr->left)
                                pcurr->balance = pcurr->balance - 1;
                            else
                                pcurr->balance = pcurr->balance + 1;
                            return curr;
                        }
                    }
                }
            }
        }
    }
    return curr;
}

int Two_Children(Tnode **curr, Tnode **pcurr){
    int pkey;
    int origin_bal;
    if(((*curr)->right) != NULL){
        origin_bal = (*curr)->balance;
        pkey = Two_Children(&((*curr)->right), curr);
        if(origin_bal == 0){ // Height of subtree doesn't change.
            return pkey; 
        }
        else{
            if((((*curr)->balance) == 1) || (((*curr)->balance) == -1)){ // Height of subtree doesn't change.
                return pkey; 
            }
            else if((((*curr)->balance) == 0)){ // Height of subtree change short.
                (*pcurr)->balance = (*pcurr)->balance + 1;
                return pkey;
            }
            else{
                if(((*curr)->balance) == 2){ // The subtree is unbalanced
                    *curr = Rotation(*curr, (*curr)->left);
                }
                else{
                    *curr = Rotation(*curr, (*curr)->right);
                }
                if((((*curr)->balance) == 1) || (((*curr)->balance) == -1)){ // Height of subtree doesn't change.
                    return pkey; 
                }
                else{ // Height of subtree change short.
                    (*pcurr)->balance = (*pcurr)->balance + 1;
                    return pkey;
                }
            }
        }
    }
    else{
        pkey = (*curr)->key;
        if(((*curr)->left) == NULL){ // The deleted node has no child
            free(*curr);
            (*pcurr)->balance = (*pcurr)->balance + 1;
            (*pcurr)->right = NULL;
            return pkey;
        }
        else{ // The deleted node has one child
            (*curr)->key = ((*curr)->left)->key;
            (*curr)->balance = 0;
            (*pcurr)->balance = (*pcurr)->balance + 1;
            free((*curr)->left);
            (*curr)->left = NULL;
            return pkey;
        }
    }
}