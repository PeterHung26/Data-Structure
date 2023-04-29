#include <stdio.h>
#include <stdlib.h>
#include "shell_array.h"
#include "shell_list.h"
#include <string.h>

int main(int argc, char *argv[])
{
    int size = 0;
    long n_comp = 0;
    int save = 0;
    if ((strcmp(argv[1], "-a") == 0) && (argc == 4))
    {
        long *array = Array_Load_From_File(argv[2], &size);
        if (array == NULL)
        {
            save = Array_Save_To_File(argv[3], array, size);
            return EXIT_FAILURE;
        }
        Array_Shellsort(array, size, &n_comp);
        save = Array_Save_To_File(argv[3], array, size);
        if(save == -1){
            free(array);
            return EXIT_FAILURE;
        }
        printf("%ld\n", n_comp);
        free(array);
    }
    else if ((strcmp(argv[1], "-l") == 0) && (argc == 4))
    {
        Node *head = NULL;
        Node *current = NULL;
        Node *prior = NULL;
        head = List_Load_From_File(argv[2]);
        head = List_Shellsort(head, &n_comp);
        save = List_Save_To_File(argv[3], head);
        printf("%ld\n", n_comp);
        current = head;
        while (current != NULL)
        {
            prior = current;
            current = current->next;
            free(prior);
        }
    }
    else
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}