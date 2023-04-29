#include <stdio.h>
#include <stdlib.h>
#include "sequence.h"

long *Generate_2p3q_Seq(int n, int *seq_size)
{
    int headp = 0;
    int tailp = 0;
    int headb = 1;
    int tailb = 1;
    int num = 0;
    int est = 0;
    int i, j;
    int curr;
    long currn;
    int index;
    long temp;
    while(1) // find head boundary
    {
        if(n <= headb)
        {
            headp--;
            headb = headb/2;
            break;
        }
        headp++;
        headb = headb * 2;
    }
    while(1) // find tail boundary
    {
        if(n <= tailb)
        {
            tailp--;
            tailb = tailb/3;
            break;
        }
        tailp++;
        tailb = tailb * 3;
    }
    if(headp == -1) // n = 1 or 0
    {
        *seq_size = num;
        long *seq = (long *)malloc(sizeof(long)*num);
        return seq;
    }
    num = (tailp+1)*(tailp+2)/2; // number of elements of complete triangle;
    est = num + (headp-tailp+1)*(headp-tailp); // estimation of intergers in the sequence
    long *seq = (long *)malloc(sizeof(long)*est);
    if(seq == NULL) // If malloc fails, size of seq = 0 and return NULL
    {
        *seq_size = 0;
        return NULL;
    }
    index = 0; // record the index of sequence
    currn = 1;
    for(i = 0;i<=tailp;i++) // put the integer in the complete triangle into the seq
    {
        temp = currn;
        for(curr = i; curr>=0; curr--)
        {
            seq[index] = temp;
            temp = temp/2*3;
            index = index + 1;
        }
        currn = currn * 2;
    }
    if(headp != tailp) //  The remaining integer which  is not a complete triangle
    {
        curr = tailp + 1;
        while(1) // find the remaining elements and put them into seq
        {
            temp = currn;
            while(1)
            {
                if(temp < n)
                {
                    num++;
                    seq[index] = temp;
                    index++;
                    temp = temp / 2 * 3;
                    continue;
                }
                else
                {
                    break;
                }
            }
            if(curr<headp)
            {
                curr++;
                currn = currn * 2;
            } 
            else
            {
                break;
            }
        }
    }
    *seq_size = num;
    for(j = num-1; j>0; j--) //Insertion sort sentienl (find min item)
    {
        if(seq[j] < seq[j-1])
        {
            temp = seq[j];
            seq[j] = seq[j-1];
            seq[j-1] = temp;
        }
    }
    for(j = 2 ; j<num; j++)
    {
        temp = seq[j];
        i = j;
        while(seq[i-1] > temp)
        {
            temp = seq[i];
            seq[i] = seq[i-1];
            seq[i-1] = temp;
            i--;
        }
        seq[i] = temp;
    }
    return seq;
}