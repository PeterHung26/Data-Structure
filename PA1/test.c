#include <stdio.h>
#include <stdlib.h>
#include "shell_array.h"
//#include "shell_list.h"

int main(){
    int size = 0;
    long n_comp = 0;
    int i;
    long *array = Array_Load_From_File("15.b", &size);
    /*for(i = 0; i < size; i++)
    {
        printf("The %d integer in the unsorted array: %ld\n", i, array[i]);
    }*/
    Array_Shellsort(array, size, &n_comp);
    /*for(i = 0; i < size; i++)
    {
        printf("integer %d is %ld\n", i, array[i]);
    }*/
    Array_Save_To_File("array.b", array, size);
    printf("\nNumber of compare is %ld\n", n_comp);
    free(array);
    /*long n_comp = 0;
    Node *head = NULL;
    Node *current = NULL;
    Node *prior = NULL;
    int i;
    head = List_Load_From_File("15.b");
    current = head;
    i = 0;
    while (current != NULL)
    {
        printf("The %d integer in the list is %ld\n", i, current->value);
        current = current->next;
        i++;
    }
    //head = List_Shellsort(head, &n_comp);
    List_Save_To_File("list.b", head);
    printf("\nNumber of compare is %ld\n", n_comp);
    current = head;
    while (current != NULL)
    {
        prior = current;
        current = current->next;
        free(prior);
    }*/

}