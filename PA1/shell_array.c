#include <stdio.h>
#include <stdlib.h>
#include "shell_array.h"
#include "sequence.h"

static int Find_k_in_sequence(long tar, long *seq, int seq_size);

static void Subarray_sort(long k, int size, long *n_comp, long *array);

long *Array_Load_From_File(char *filename, int *size)
{
    FILE *in = NULL;
    in = fopen(filename, "rb");
    if (in == NULL) // fail to open the file
    {
        *size = 0;
        return NULL;
    }
    fseek(in, 0, SEEK_END);
    long file_size = ftell(in); // find the file size
    rewind(in);
    long *file_read = (long *)malloc(file_size);
    if (file_read == NULL) // fail to allocate the memory
    {
        *size = 0;
        fclose(in);
        return NULL;
    }
    int longsize = sizeof(long);
    *size = file_size / longsize; // long is 8 bytes. As a result, divide file_size by 8
    if (file_size == 0)
    {
        fclose(in);
        return file_read;
    }
    int i = 0;
    while (fread(file_read + i, longsize, 1, in) == 1) // read the file
    {
        i++;
    }
    fclose(in);
    return file_read;
}

int Array_Save_To_File(char *filename, long *array, int size)
{
    FILE *out = NULL;
    out = fopen(filename, "wb");
    if(out == NULL)
        return -1;
    int i = 0;
    int longsize = sizeof(long);
    int success = 0; // number of long integers successfully written in file
    if ((array == NULL) || (size == 0))
    {
        fclose(out);
        return success;
    }
    for (i = 0; i < size; i++)
    {
        if (fwrite(array + i, longsize, 1, out) == 1)
        {
            success++;
        }
    }
    fclose(out);
    return success;
}

void Array_Shellsort(long *array, int size, long *n_comp)
{
    int i, j, k;
    int seq_size = 0;
    long *seq = Generate_2p3q_Seq(size, &seq_size);
    *n_comp = 0;
    long temp;
    int find2;
    int find3;
    for(k = seq_size-1; k >= 0; k--)
    {
        if((k<(seq_size-2)))
        {
            temp = seq[k]*2;
            find2 = Find_k_in_sequence(temp, seq, seq_size);
            temp = seq[k]*3;
            find3 = Find_k_in_sequence(temp, seq, seq_size);
            if((find2 == 1) && (find3 == 1)) // Every integer in the subarray is at most one correct position away
            {
                for(j = size - 1; j >= (size - seq[k]); j--)
                {
                    i = j;
                    while((i-seq[k]) >= 0)
                    {
                        if(array[i] < array[i-seq[k]])
                        {
                            *n_comp = *n_comp + 1;
                            temp = array[i];
                            array[i] = array[i-seq[k]];
                            array[i-seq[k]] = temp;
                            i = i - 2*seq[k];
                        }
                        else
                        {
                            *n_comp = *n_comp + 1;
                            i = i - seq[k];
                        }
                    }
                }
            }
            else
            {
                Subarray_sort(seq[k], size, n_comp, array);
            }
        }
        else
        {
            Subarray_sort(seq[k], size, n_comp, array);
        }
    }
    free(seq);
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
            //printf("\nFind %ld at seq[%d]\n", tar, middle);
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
    /*if(find == 0)
    {
        printf("\nDoesn't find %ld\n", tar);
    }*/
    return find;
}

void Subarray_sort(long k, int size, long *n_comp, long *array)
{
    int i, j;
    long temp;
    int sorted;
    int last_exchange;
    int last_element;
    int l;
    for (j = size - 1; j >= size - k; j--)
    {
        l = j % k;
        sorted = 0;
        last_exchange = j;
        while(!sorted)
        {
            sorted = 1;
            last_element = last_exchange;
            for(i = l + k;i <= last_element; i = i + k)
            {
                if(array[i-k] > array[i])
                {
                    temp = array[i-k];
                    array[i-k] = array[i];
                    array[i] = temp;
                    last_exchange = i;
                    sorted = 0;
                    *n_comp = *n_comp + 1;
                }
                else
                {
                    *n_comp = *n_comp + 1;
                }
            }
        }
    }
}