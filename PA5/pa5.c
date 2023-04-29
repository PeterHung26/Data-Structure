#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"vhnode.h"
#include"loading.h"

int main(int argc, char *argv[]){
    FILE *fp;
    int M, N;
    int hrow, hcol;
    int vrow, vcol;
    Node *vboard;
    Node *hboard;
    PQ *vheap;
    PQ *hheap;
    //int i;
    if(argc == 4){
        fp = fopen ( argv[1], "r" ) ;
        if(fp == NULL)
            return EXIT_FAILURE;
        ROW_AND_COL(fp, &M, &N);
        vrow = M - 1;
        vcol = N;
        hrow = M;
        hcol = N + 1;
        vboard = (Node *)malloc(sizeof(Node) * (vrow * vcol));
        hboard = (Node *)malloc(sizeof(Node) * (hrow * hcol));
        vheap = (PQ *)malloc(sizeof(PQ) * (vrow * vcol));
        hheap = (PQ *)malloc(sizeof(PQ) * (hrow * hcol));
        RESET_VALUE(vboard, vrow, vcol, hboard, hrow, hcol, vheap, hheap);
        if(SET_BOARD(fp, vboard, vrow, vcol)){
            free(vboard);
            free(hboard);
            free(hheap);
            free(vheap);
            fclose(fp);
            return EXIT_FAILURE;
        }
        fclose(fp);
        CAL_TOTAL_SHORT(vboard, vrow, vcol, hboard, hrow, hcol, vheap, hheap);
        /*for(i = 0; i < hrow; i++){
            printf("%d\n", hboard[i*hcol + hcol - 1].short_weight);
        }*/
        int shortest_row;
        shortest_row = SEARCH_SHORTEST(hboard, hrow, hcol);
        fp = fopen ( argv[2], "w" ) ;
        PRINT_PATH(fp, shortest_row, hcol - 1, 'H', vboard, vrow, vcol, hboard, hrow, hcol);
        fclose(fp);
        fp = fopen ( argv[3], "w" ) ;
        fclose(fp);
        free(vboard);
        free(hboard);
        free(hheap);
        free(vheap);
    }
    else
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}