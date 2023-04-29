#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"packing.h"

Node *Build_BTree(FILE *fp)
{
    Node *head = NULL;
    int n;
    char c;
    char temp[10];
    n = 0;
    while(1) // Find the Label
    {
        c = fgetc(fp);
        if(c != '\n' && c!= '(' && c != EOF)
        {
            n++; // Find the size of label
        }
        else
        {
            break;
        }
    }
    fseek(fp, -(n + 1), SEEK_CUR); // char is 1 byte. As a result, label byte = n
    head = (Node *)malloc(sizeof(Node));
    if(head == NULL)
        return head;
    head->label = (char *)malloc(n+1);
    fread(head->label,n,1,fp);
    head->label[n] = '\0';
    //printf("Label is %s\n",head->label);
    if(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V")))
    {
        head->wid = 0;
        head->hei = 0;
        fseek(fp, 1, SEEK_CUR);
        head->left = Build_BTree(fp);
        head->right = Build_BTree(fp);
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
        fseek(fp, 2, SEEK_CUR);
        head->left = NULL;
        head->right = NULL;
    }
    return head;
}

void Output1(Node *head, FILE *fp)
{
    if(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V")))
    {
        Output1(head->left,fp);
        Output1(head->right,fp);
        fprintf(fp,"%s\n",head->label);
    }
    else
    {
        fprintf(fp,"%s(%d,%d)\n", head->label, head->wid, head->hei);
    }
}

void Output2(Node *head, FILE *fp, int *w, int *h)
{
    if(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V")))
    {
        int wl = 0;
        int hl = 0;
        int wr = 0;
        int hr = 0;
        Output2(head->left, fp, &wl, &hl);
        Output2(head->right, fp, &wr, &hr);
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
        head->wid = *w;
        head->hei = *h;
    }
    else
    {
        *w = head->wid;
        *h = head->hei;
    }
    fprintf(fp,"%s(%d,%d)\n",head->label, *w, *h);
}

void Output3(Node *head, FILE *fp, int w, int h, int x, int y, char mode, char dir)
{
    if(!(strcmp(head->label,"H")) || !(strcmp(head->label,"V")))
    {
        if(mode == '0')
        {
            Output3(head->left, fp, head->wid, head->hei, 0, 0, head->label[0], 'l');
            Output3(head->right, fp, head->wid, head->hei, 0, 0, head->label[0], 'r');
        }
        else
        {
            if(dir == 'l')
            {
                if(mode == 'H')
                {
                    Output3(head->left, fp, head->wid, head->hei, x, y+h-head->hei, head->label[0], 'l');
                    Output3(head->right, fp, head->wid, head->hei, x, y+h-head->hei, head->label[0], 'r');
                }
                else
                {
                    Output3(head->left, fp, head->wid, head->hei, x, y, head->label[0], 'l');
                    Output3(head->right, fp, head->wid, head->hei, x, y, head->label[0], 'r');
                }
            }
            else
            {
                if(mode == 'H')
                {
                    Output3(head->left, fp, head->wid, head->hei, x, y, head->label[0], 'l');
                    Output3(head->right, fp, head->wid, head->hei, x, y, head->label[0], 'r');
                }
                else
                {
                    Output3(head->left, fp, head->wid, head->hei, x+w-head->wid, y, head->label[0], 'l');
                    Output3(head->right, fp, head->wid, head->hei, x+w-head->wid, y, head->label[0], 'r');
                }
            }
        }
    }
    else
    {
        if(mode == 'H')
        {
            if(dir == 'l')
            {
                fprintf(fp,"%s((%d,%d)(%d,%d))\n",head->label, head->wid, head->hei, x, y+h-(head->hei));
            }
            else
            {
                fprintf(fp,"%s((%d,%d)(%d,%d))\n",head->label, head->wid, head->hei, x, y);
            }
        }
        else
        {
            if(dir == 'l')
            {
                fprintf(fp,"%s((%d,%d)(%d,%d))\n",head->label, head->wid, head->hei, x, y);
            }
            else
            {
                fprintf(fp,"%s((%d,%d)(%d,%d))\n",head->label, head->wid, head->hei, x+w-head->wid, y);
            }
        }
    }
}