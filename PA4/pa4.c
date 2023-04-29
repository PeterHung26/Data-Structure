#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"hbt.h"
#include"evaluation.h"
#include"building.h"
static void Memory_Free(Tnode *head);
static int Output_Building(Tnode *head, FILE *fp);

int main(int argc, char *argv[]){
    FILE *fp;
    Tnode *head = NULL;
    long size;
    if(!(strcmp(argv[1], "-b")) && (argc == 4)){
        int build = 1;
        if(argv[2] != NULL){
            fp = fopen ( argv[2], "rb" );
            if(fp == NULL){
                build = -1;
                fprintf(stdout, "%d\n", build);
                return EXIT_FAILURE;
            }
        }
        else
            return EXIT_FAILURE;
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        size = size / (sizeof(int) + sizeof(char)); // Find how many operation
        rewind(fp);
        int i;
        int keyy;
        char op;
        for(i = 0; i < size; i++){
            if(!fread(&keyy, sizeof(int), 1, fp)){
                build = 0;
                break;
            }
            if(!fread(&op, sizeof(char), 1, fp)){
                build = 0;
                break;
            }
            if(op == 'i'){
                head = Insertion(head, keyy, &build);
                if(build == 0){
                    fprintf(stdout, "%d\n", build);
                    fclose(fp);
                    Memory_Free(head);
                    return EXIT_FAILURE;
                }
            }
            else if(op == 'd'){
                head = Deletion(head, keyy, &build);
            }
            else{
                build = 0;
                fprintf(stdout, "%d\n", build);
                fclose(fp);
                Memory_Free(head);
                return EXIT_FAILURE;
            }
        }
        fclose(fp);
        if(argv[3] != NULL){
            fp = fopen ( argv[3], "wb" );
            if(fp == NULL){
                build = 0;
                fprintf(stdout, "%d\n", build);
                return EXIT_FAILURE;
            }
        }
        else
            return EXIT_FAILURE;
        Output_Building(head, fp);
        fclose(fp);
        Memory_Free(head);
        fprintf(stdout, "%d\n", build);
        return EXIT_SUCCESS;
    }
    else if(!(strcmp(argv[1], "-e")) && (argc == 3)){
        int valid = 0;
        int bst = 0;
        int hbtt = 0;
        if(argv[2] != NULL){
            fp = fopen ( argv[2], "rb" );
            if(fp == NULL){
                valid = -1;
                fprintf(stdout, "%d,0,0\n", valid);
                return EXIT_FAILURE;
            }
        }
        else
            return EXIT_FAILURE;
        valid = Read_File_Eval(fp, &head);
        fclose (fp) ;
        if(valid == 0){
            fprintf(stdout, "%d,0,0\n", valid);
            return EXIT_FAILURE;
        }
        bst = BST_Eval(head);
        hbtt = HBT_Eval(head);
        fprintf(stdout, "%d,%d,%d\n", valid, bst, hbtt);
        Memory_Free(head);
        return EXIT_SUCCESS;
    }
    else
        return EXIT_FAILURE;
}

void Memory_Free(Tnode *head){
    if(head == NULL)
        return;
    if(head->left != NULL)
        Memory_Free(head->left);
    if(head->right != NULL)
        Memory_Free(head->right);
    free(head);
}

int Output_Building(Tnode *head, FILE *fp){
    int temp = head->key;
    char temp2;
    if((head->left != NULL) && (head->right != NULL)){
        if (!fwrite(&temp, sizeof(int), 1, fp))
            return 1;
        temp2 = 3;
        if (!fwrite(&temp2, sizeof(char), 1, fp))
            return 1;
        if(Output_Building(head->left, fp))
            return 1;
        if(Output_Building(head->right, fp))
            return 1;
    }
    else if(head->left != NULL){
        if (!fwrite(&temp, sizeof(int), 1, fp))
            return 1;
        temp2 = 2;
        if (!fwrite(&temp2, sizeof(char), 1, fp))
            return 1;
        if(Output_Building(head->left, fp))
            return 1;
    }
    else if(head->right != NULL){
        if (!fwrite(&temp, sizeof(int), 1, fp))
            return 1;
        temp2 = 1;
        if (!fwrite(&temp2, sizeof(char), 1, fp))
            return 1;
        if(Output_Building(head->right, fp))
            return 1;
    }
    else{
        if (!fwrite(&temp, sizeof(int), 1, fp))
            return 1;
        temp2 = 0;
        if (!fwrite(&temp2, sizeof(char), 1, fp))
            return 1;
    }
    return 0;
}