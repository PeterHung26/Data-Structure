#include <stdio.h>
#include <stdlib.h>
#include "shell_list.h"
#include "sequence.h"

static Node *Find_The_K_Node(Node *list, long k);

static int Find_k_in_sequence(long tar, long *seq, int seq_size);

static Node *Sublist_sort(long k, int size, long *n_comp, Node *list);

Node *List_Load_From_File(char *filename)
{
    FILE *in = NULL;
    in = fopen(filename, "rb");
    if (in == NULL) // fail to open the file
    {
        return NULL;
    }
    fseek(in, 0, SEEK_END);
    long file_size = ftell(in); // find the file size
    rewind(in);
    long size = file_size / sizeof(long); // How many elements in the binary file
    Node *head = NULL;
    Node *current = NULL;
    int i = 0;
    for (i = 0; i < size; i++)
    {
        if (head == NULL)
        {
            head = (Node *)malloc(sizeof(Node));
            current = head;
        }
        else
        {
            current->next = (Node *)malloc(sizeof(Node));
            current = current->next;
        }
        if(current == NULL)
        {
            break;
        }
        fread(&(current->value), sizeof(long), 1, in);
        current->next = NULL;
    }
    fclose(in);
    return head;
}

int List_Save_To_File(char *filename, Node *list)
{
    FILE *out = NULL;
    out = fopen(filename, "wb");
    if(out == NULL)
        return -1;
    int longsize = sizeof(long);
    int success = 0; // number of long integers successfully written in file
    while (list != NULL)
    {
        if (fwrite(&(list->value), longsize, 1, out) == 1)
        {
            success++;
        }
        list = list->next;
    }
    fclose(out);
    return success;
}

Node *List_Shellsort(Node *list, long *n_comp)
{
    Node *current = NULL;
    Node *before_current = NULL;
    Node *prior = NULL;
    Node *before_prior = NULL;
    Node *temp = NULL;
    Node dummy;
    int seq_size = 0;
    int j, k;
    int find2, find3;
    current = list;
    int size = 0;
    while (current != NULL) // Find how many integer in the linked list
    {
        size++;
        current = current->next;
    }
    long *seq = Generate_2p3q_Seq(size, &seq_size);
    for(k = seq_size-1; k >= 0; k--)
    {
        if((k<(seq_size-2)))
        {
            find2 = Find_k_in_sequence(seq[k]*2, seq, seq_size);
            find3 = Find_k_in_sequence(seq[k]*3, seq, seq_size);
            if((find2 == 1) && (find3 == 1)) // Every integer in the sublist is at most one correct position away
            {
                dummy.next = list;
                j = seq[k];
                before_prior = &dummy;
                prior = before_prior->next;
                before_current = Find_The_K_Node(before_prior, seq[k]);
                if(before_current != NULL)
                {
                    current = before_current->next;
                }
                else
                {
                    current = NULL;
                }
                while((j < size) && (current != NULL) && (prior != NULL)) // Doing sorting of each sublist
                {
                    if(current->value < prior->value)
                    {
                        *n_comp = *n_comp + 1;
                        temp = before_current->next;
                        before_current->next = before_prior->next;
                        before_prior->next = temp;
                        temp = current->next;
                        current->next = prior->next;
                        prior->next = temp;
                        j = j + 1;
                        before_prior = before_prior->next;
                        if (before_prior != NULL)
                        {
                            prior = before_prior->next;
                        }
                        else
                        {
                            prior = NULL;
                        }
                        if(seq[k] == 1)
                        {
                            if(before_current != NULL)
                            {
                                current = before_current->next;
                            }
                            else
                            {
                                current = NULL;
                            }
                        }
                        else
                        {
                            before_current = before_current->next;
                            if (before_current != NULL)
                            {
                                current = before_current->next;
                            }
                            else
                            {
                                current = NULL;
                            }
                        }
                        
                    }
                    else
                    {
                        *n_comp = *n_comp + 1;
                        j = j + 1;
                        before_prior = before_prior->next;
                        if (before_prior != NULL)
                        {
                            prior = before_prior->next;
                        }
                        else
                        {
                            prior = NULL;
                            continue;
                        }
                        before_current = before_current->next;
                        if (before_current != NULL)
                        {
                            current = before_current->next;
                        }
                        else
                        {
                            current = NULL;
                            continue;
                        }
                    }
                }
                list = dummy.next;
            }
            else
            {
                list = Sublist_sort(seq[k], size, n_comp, list);
            }
        }
        else
        {
            list = Sublist_sort(seq[k], size, n_comp, list);
        }
    }
    free(seq);
    return list;
}

Node *Find_The_K_Node(Node *list, long k)
{
    int i = 0;
    Node *current = NULL;
    current = list;
    for(i = 0; i < k; i++)
    {
        current = current->next;
        if(current == NULL)
        {
            return NULL;
        }
    }
    return current;
}

int Find_k_in_sequence(long tar, long *seq, int seq_size)
{
    int left = 0;
    int right = seq_size-1;
    int middle = (left + right)/2;
    int find = 0;
    while((left <= right) && (find == 0))
    {
        if(tar == seq[middle])
        {
            find = 1;
            return find;
        }
        else
        {
            if(seq[middle] > tar)
            {
                right = middle - 1;
                middle = (left + right)/2;
                continue;
            }
            else
            {
                left = middle + 1;
                middle = (left + right)/2;
                continue;
            }
        }
    }
    return find;
}

Node *Sublist_sort(long k, int size, long *n_comp, Node *list)
{
    int i, j;
    int sorted;
    int last_element;
    Node *temp = NULL;
    Node *current = NULL;
    Node *before_current = NULL;
    Node *prior = NULL;
    Node *before_prior = NULL;
    Node dummy;
    dummy.next = list;
    sorted = 0;
    j = size - 1;
    while(!sorted && (j >= k))
    {
        sorted = 1;
        last_element = j;
        i = 1;
        before_prior = &dummy;
        if (before_prior != NULL)
        {
            prior = before_prior->next;
        }
        else
        {
            prior = NULL;
        }
        before_current = Find_The_K_Node(before_prior, k);
        if (before_current != NULL)
        {
            current = before_current->next;
        }
        else
        {
            current = NULL;
        }
        while((current != NULL) && (i <= last_element) && (prior != NULL))
        {
            if(current->value < prior->value)
            {
                temp = before_current->next;
                before_current->next = before_prior->next;
                before_prior->next = temp;
                temp = current->next;
                current->next = prior->next;
                prior->next = temp;
                sorted = 0;
                *n_comp = *n_comp + 1;
                i = i + 1;
                if (before_prior != NULL)
                {
                    prior = before_prior->next;
                }
                else
                {
                    prior = NULL;
                }
                if(k == 1)
                {
                    if(before_current != NULL)
                    {
                        current = before_current->next;
                    }
                    else
                    {
                        current = NULL;
                    }
                }
                else
                {
                    before_current = before_current->next;
                    if (before_current != NULL)
                    {
                        current = before_current->next;
                    }
                    else
                    {
                        current = NULL;
                    }
                }
            }
            else
            {
                *n_comp = *n_comp + 1;
                i = i + 1;
                before_prior = before_current;
                prior = before_prior->next;
                before_current = before_current->next;
                if(before_current != NULL)
                {
                    current = before_current->next;
                }
                else
                {
                    current = NULL;
                }
            }
        }
        j = j - k;
    }
    
    

    list = dummy.next;
    return list;
}