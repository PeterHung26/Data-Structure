#ifndef __EVALUATION__
#define __EVALUATION__
#include<stdio.h>
#include"hbt.h"

int Read_File_Eval(FILE *fp, Tnode **head);

int BST_Eval(Tnode *head);

int HBT_Eval(Tnode *head);
#endif