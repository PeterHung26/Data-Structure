#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include"vhnode.h"

static void EXTRACT_MIN(PQ *heapp, Node *board, int* size, int bcol);
static void DOWNWARD_HEAPIFY(PQ *heapp, Node *board, int size, int bcol);
static void UPWARD_HEAPIFY(PQ *heapp, Node *board, int bcol, int index);
static void UPDATA_SHORT_FROM_HBOARD(PQ *vheap, Node *hboard, Node *vboard, int hcol, int vcol, int vrow, int rrow, int ccol);
static void UPDATA_SHORT_FROM_VBOARD(PQ *hheap,PQ *vheap, Node *hboard, Node *vboard, int hcol, int vcol, int vrow, int rrow, int ccol);

void ROW_AND_COL(FILE *fp, int *M, int *N){
    char c;
    char temp[10];
    int n;
    n = 0;
    while(1){
        c = fgetc(fp);
        if(c != ' '){
            n++; // Find the size of label
        }
        else{
            break;
        }
    }
    fseek(fp, -(n + 1), SEEK_CUR);
    fread(temp,n,1,fp);
    temp[n] = '\0';
    *M = atoi(temp);
    fseek(fp, 1, SEEK_CUR);
    n = 0;
    while(1){
        c = fgetc(fp);
        if(c != '\n'){
            n++; // Find the size of label
        }
        else{
            break;
        }
    }
    fseek(fp, -(n + 1), SEEK_CUR);
    fread(temp,n,1,fp);
    temp[n] = '\0';
    *N = atoi(temp);
    fseek(fp, 1, SEEK_CUR);
    return;
}

void RESET_VALUE(Node *vboard, int vrow, int vcol, Node *hboard, int hrow, int hcol, PQ *vheap, PQ *hheap){
    int i, j;
    PQ temp;
    for(i = 0; i < vrow; i++){
        for(j = 0; j < vcol; j++){
            ((vboard + i*vcol) + j)->weight = 1;
            ((vboard + i*vcol) + j)->short_weight = INT_MAX;
            ((vboard + i*vcol) + j)->pred_row = -1;
            ((vboard + i*vcol) + j)->pred_col = -1;
            ((vboard + i*vcol) + j)->heap_index = i*vcol + j;
            ((vboard + i*vcol) + j)->pred = 'N';
            ((vheap + i*vcol) + j)->row = i;
            ((vheap + i*vcol) + j)->col = j;
            ((vheap + i*vcol) + j)->distance = INT_MAX;
        }
    }
    for(i = 0; i < hrow; i++){
        for(j = 0; j < hcol; j++){
            if(j == 0){
                ((hboard + i*hcol) + j)->weight = 0;
                ((hboard + i*hcol) + j)->short_weight = 0;
                ((hheap + i*hcol) + j)->distance = 0;
            }
            else{
                ((hboard + i*hcol) + j)->weight = 1;
                ((hboard + i*hcol) + j)->short_weight = INT_MAX;
                ((hheap + i*hcol) + j)->distance = INT_MAX;
            }
            ((hboard + i*hcol) + j)->pred_row = -1;
            ((hboard + i*hcol) + j)->pred_col = -1;
            ((hboard + i*hcol) + j)->heap_index = i*hcol + j;
            ((hboard + i*hcol) + j)->pred = 'N';
            ((hheap + i*hcol) + j)->row = i;
            ((hheap + i*hcol) + j)->col = j;
        }
    }
    for(i = 1; i < hrow; i++){ // Upward heapify hheap
        hboard[i*hcol].heap_index = i; // Updata heap index
        hboard[i].heap_index = i*hcol;
        temp = hheap[i*hcol];
        hheap[i*hcol] = hheap[i];
        hheap[i] = temp;
    }
    return;
}

int SET_BOARD(FILE *fp, Node *vboard, int vrow, int vcol){
    char c;
    int i, j;
    for(i = 0; i < vrow; i++){
        for(j = 0; j< vcol; j++){
            c = fgetc(fp);
            if(c == '1')
                ((vboard + i*vcol) + j)->weight = 0;
            else if(c == '0')
                continue;
            else
                return 1;
        }
        fseek(fp, 1, SEEK_CUR);
    }
    return 0;
}

void CAL_TOTAL_SHORT(Node *vboard, int vrow, int vcol, Node *hboard, int hrow, int hcol, PQ *vheap, PQ *hheap){
    int hsize = hrow * hcol;
    int vsize = vrow * vcol;
    int rrow;
    int ccol;
    while((hsize != 0) || (vsize != 0)){
        if((hsize != 0) && (vsize != 0)){
            if((hheap)->distance <= (vheap)->distance){
                rrow = hheap[0].row;
                ccol = hheap[0].col;
                EXTRACT_MIN(hheap, hboard, &hsize, hcol);
                UPDATA_SHORT_FROM_HBOARD(vheap, hboard, vboard, hcol, vcol, vrow, rrow, ccol);
            }
            else{
                rrow = vheap[0].row;
                ccol = vheap[0].col;
                EXTRACT_MIN(vheap, vboard, &vsize, vcol);
                UPDATA_SHORT_FROM_VBOARD(hheap, vheap, hboard, vboard, hcol, vcol, vrow, rrow, ccol);
            }
        }
        else if(hsize == 0){
            rrow = vheap[0].row;
            ccol = vheap[0].col;
            EXTRACT_MIN(vheap, vboard, &vsize, vcol);
            UPDATA_SHORT_FROM_VBOARD(hheap, vheap, hboard, vboard, hcol, vcol, vrow, rrow, ccol);
        }
        else{
            rrow = hheap[0].row;
            ccol = hheap[0].col;
            EXTRACT_MIN(hheap, hboard, &hsize, hcol);
            UPDATA_SHORT_FROM_HBOARD(vheap, hboard, vboard, hcol, vcol, vrow, rrow, ccol);
        }
    }
    return;
}

int SEARCH_SHORTEST(Node *hboard, int hrow, int hcol){
    int i;
    int shortest = INT_MAX;
    int row;
    for(i = 0; i < hrow; i++){
        if((hboard + i*hcol + hcol - 1)->short_weight < shortest){
            shortest = (hboard + i*hcol + hcol - 1)->short_weight;
            row = i;
        }
    }
    return row;
}

void PRINT_PATH(FILE *fp, int rrow, int ccol, char typpe, Node *vboard, int vrow, int vcol, Node *hboard, int hrow, int hcol){
    int nxt_row, nxt_col;
    char nxt_type;
    if(typpe == 'H'){
        nxt_row = hboard[rrow*hcol + ccol].pred_row;
        nxt_col = hboard[rrow*hcol + ccol].pred_col;
        nxt_type = hboard[rrow*hcol + ccol].pred;
        if(nxt_type != 'N')
            PRINT_PATH(fp, nxt_row, nxt_col, nxt_type, vboard, vrow, vcol, hboard, hrow, hcol);
        fprintf(fp, "(%d,%d)(%d,%d)\n", rrow, ccol-1, rrow, ccol);
    }
    else if(typpe == 'V'){
        nxt_row = vboard[rrow*vcol + ccol].pred_row;
        nxt_col = vboard[rrow*vcol + ccol].pred_col;
        nxt_type = vboard[rrow*vcol + ccol].pred;
        if(nxt_type != 'N')
            PRINT_PATH(fp, nxt_row, nxt_col, nxt_type, vboard, vrow, vcol, hboard, hrow, hcol);
        fprintf(fp, "(%d,%d)(%d,%d)\n", rrow, ccol, rrow+1, ccol);
    }
    return;
}

void EXTRACT_MIN(PQ *heapp, Node *board, int* size, int bcol){
    PQ temp;
    int row1 = heapp[0].row;
    int col1 = heapp[0].col;
    int row2 = heapp[*size - 1].row;
    int col2 = heapp[*size - 1].col;
    board[row1*bcol + col1].heap_index = *size - 1; // Update the heap index
    board[row2*bcol + col2].heap_index = 0; // Update the heap index
    temp = heapp[0];
    heapp[0] = heapp[*size - 1];
    heapp[*size - 1] = temp;
    *size = *size - 1;
    DOWNWARD_HEAPIFY(heapp, board, *size, bcol);
    return;
}

void UPDATA_SHORT_FROM_HBOARD(PQ *vheap, Node *hboard, Node *vboard, int hcol, int vcol, int vrow, int rrow, int ccol){
    int index;
    if((ccol - 1) >= 0){
        if(rrow - 1 >= 0){
            if((hboard[rrow*hcol + ccol].short_weight + vboard[(rrow - 1)*vcol + (ccol - 1)].weight) < vboard[(rrow - 1)*vcol + (ccol - 1)].short_weight){
                vboard[(rrow - 1)*vcol + (ccol - 1)].short_weight = hboard[rrow*hcol + ccol].short_weight + vboard[(rrow - 1)*vcol + (ccol - 1)].weight;
                vboard[(rrow - 1)*vcol + (ccol - 1)].pred_col = ccol;
                vboard[(rrow - 1)*vcol + (ccol - 1)].pred_row = rrow;
                vboard[(rrow - 1)*vcol + (ccol - 1)].pred = 'H';
                index = vboard[(rrow - 1)*vcol + (ccol - 1)].heap_index;
                vheap[index].distance = vboard[(rrow - 1)*vcol + (ccol - 1)].short_weight;
                UPWARD_HEAPIFY(vheap, vboard, vcol, index);
            }
        }
        if(rrow < vrow){
            if((hboard[rrow*hcol + ccol].short_weight + vboard[rrow*vcol + (ccol - 1)].weight) < vboard[rrow*vcol + (ccol - 1)].short_weight){
                vboard[rrow*vcol + (ccol - 1)].short_weight = hboard[rrow*hcol + ccol].short_weight + vboard[rrow*vcol + (ccol - 1)].weight;
                vboard[rrow*vcol + (ccol - 1)].pred_col = ccol;
                vboard[rrow*vcol + (ccol - 1)].pred_row = rrow;
                vboard[rrow*vcol + (ccol - 1)].pred = 'H';
                index = vboard[rrow*vcol + (ccol - 1)].heap_index;
                vheap[index].distance = vboard[rrow*vcol + (ccol - 1)].short_weight;
                UPWARD_HEAPIFY(vheap, vboard, vcol, index);
            }
        }
    }
    if(ccol < vcol){
        if(rrow - 1 >= 0){
            if((hboard[rrow*hcol + ccol].short_weight + vboard[(rrow - 1)*vcol + ccol].weight) < vboard[(rrow - 1)*vcol + ccol].short_weight){
                vboard[(rrow - 1)*vcol + ccol].short_weight = hboard[rrow*hcol + ccol].short_weight + vboard[(rrow - 1)*vcol + ccol].weight;
                vboard[(rrow - 1)*vcol + ccol].pred_col = ccol;
                vboard[(rrow - 1)*vcol + ccol].pred_row = rrow;
                vboard[(rrow - 1)*vcol + ccol].pred = 'H';
                index = vboard[(rrow - 1)*vcol + ccol].heap_index;
                vheap[index].distance = vboard[(rrow - 1)*vcol + ccol].short_weight;
                UPWARD_HEAPIFY(vheap, vboard, vcol, index);
            }
        }
        if(rrow < vrow){
            if((hboard[rrow*hcol + ccol].short_weight + vboard[rrow*vcol + ccol].weight) < vboard[rrow*vcol + ccol].short_weight){
                vboard[rrow*vcol + ccol].short_weight = hboard[rrow*hcol + ccol].short_weight + vboard[rrow*vcol + ccol].weight;
                vboard[rrow*vcol + ccol].pred_col = ccol;
                vboard[rrow*vcol + ccol].pred_row = rrow;
                vboard[rrow*vcol + ccol].pred = 'H';
                index = vboard[rrow*vcol + ccol].heap_index;
                vheap[index].distance = vboard[rrow*vcol + ccol].short_weight;
                UPWARD_HEAPIFY(vheap, vboard, vcol, index);
            }
        }
    }
}

void UPDATA_SHORT_FROM_VBOARD(PQ *hheap, PQ *vheap, Node *hboard, Node *vboard, int hcol, int vcol, int vrow, int rrow, int ccol){
    int index;
    if((rrow - 1) >= 0){
        if(vboard[(rrow - 1)*vcol + ccol].weight == 0){
            if(vboard[rrow*vcol + ccol].short_weight < vboard[(rrow - 1)*vcol + ccol].short_weight){
                vboard[(rrow - 1)*vcol + ccol].short_weight = vboard[rrow*vcol + ccol].short_weight;
                vboard[(rrow - 1)*vcol + ccol].pred = 'V';
                vboard[(rrow - 1)*vcol + ccol].pred_col = ccol;
                vboard[(rrow - 1)*vcol + ccol].pred_row = rrow;
                index = vboard[(rrow - 1)*vcol + ccol].heap_index;
                vheap[index].distance = vboard[(rrow - 1)*vcol + ccol].short_weight;
                UPWARD_HEAPIFY(vheap, vboard, vcol, index);
            }
        }
    }
    if((rrow + 1) < vrow){
        if(vboard[(rrow + 1)*vcol + ccol].weight == 0){
            if(vboard[rrow*vcol + ccol].short_weight < vboard[(rrow + 1)*vcol + ccol].short_weight){
                vboard[(rrow + 1)*vcol + ccol].short_weight = vboard[rrow*vcol + ccol].short_weight;
                vboard[(rrow + 1)*vcol + ccol].pred = 'V';
                vboard[(rrow + 1)*vcol + ccol].pred_col = ccol;
                vboard[(rrow + 1)*vcol + ccol].pred_row = rrow;
                index = vboard[(rrow + 1)*vcol + ccol].heap_index;
                vheap[index].distance = vboard[(rrow + 1)*vcol + ccol].short_weight;
                UPWARD_HEAPIFY(vheap, vboard, vcol, index);
            }
        }
    }
    if((vboard[rrow*vcol + ccol].short_weight + hboard[rrow*hcol + ccol].weight) < hboard[rrow*hcol + ccol].short_weight){
        hboard[rrow*hcol + ccol].short_weight = vboard[rrow*vcol + ccol].short_weight + hboard[rrow*hcol + ccol].weight;
        hboard[rrow*hcol + ccol].pred = 'V';
        hboard[rrow*hcol + ccol].pred_col = ccol;
        hboard[rrow*hcol + ccol].pred_row = rrow;
        index = hboard[rrow*hcol + ccol].heap_index;
        hheap[index].distance = hboard[rrow*hcol + ccol].short_weight;
        UPWARD_HEAPIFY(hheap, hboard, hcol, index);
    }
    if((vboard[rrow*vcol + ccol].short_weight + hboard[(rrow+1)*hcol + ccol].weight) < hboard[(rrow+1)*hcol + ccol].short_weight){
        hboard[(rrow+1)*hcol + ccol].short_weight = vboard[rrow*vcol + ccol].short_weight + hboard[(rrow+1)*hcol + ccol].weight;
        hboard[(rrow+1)*hcol + ccol].pred = 'V';
        hboard[(rrow+1)*hcol + ccol].pred_col = ccol;
        hboard[(rrow+1)*hcol + ccol].pred_row = rrow;
        index = hboard[(rrow+1)*hcol + ccol].heap_index;
        hheap[index].distance = hboard[(rrow+1)*hcol + ccol].short_weight;
        UPWARD_HEAPIFY(hheap, hboard, hcol, index);
    }
    if((vboard[rrow*vcol + ccol].short_weight + hboard[rrow*hcol + (ccol+1)].weight) < hboard[rrow*hcol + (ccol+1)].short_weight){
        hboard[rrow*hcol + (ccol+1)].short_weight = vboard[rrow*vcol + ccol].short_weight + hboard[rrow*hcol + (ccol+1)].weight;
        hboard[rrow*hcol + (ccol+1)].pred = 'V';
        hboard[rrow*hcol + (ccol+1)].pred_col = ccol;
        hboard[rrow*hcol + (ccol+1)].pred_row = rrow;
        index = hboard[rrow*hcol + (ccol+1)].heap_index;
        hheap[index].distance = hboard[rrow*hcol + (ccol+1)].short_weight;
        UPWARD_HEAPIFY(hheap, hboard, hcol, index);
    }
    if((vboard[rrow*vcol + ccol].short_weight + hboard[(rrow+1)*hcol + (ccol+1)].weight) < hboard[(rrow+1)*hcol + (ccol+1)].short_weight){
        hboard[(rrow+1)*hcol + (ccol+1)].short_weight = vboard[rrow*vcol + ccol].short_weight + hboard[(rrow+1)*hcol + (ccol+1)].weight;
        hboard[(rrow+1)*hcol + (ccol+1)].pred = 'V';
        hboard[(rrow+1)*hcol + (ccol+1)].pred_col = ccol;
        hboard[(rrow+1)*hcol + (ccol+1)].pred_row = rrow;
        index = hboard[(rrow+1)*hcol + (ccol+1)].heap_index;
        hheap[index].distance = hboard[(rrow+1)*hcol + (ccol+1)].short_weight;
        UPWARD_HEAPIFY(hheap, hboard, hcol, index);
    }
}

void DOWNWARD_HEAPIFY(PQ *heapp, Node *board, int size, int bcol){
    int i = 0; // parent
    int j = 2*i + 1; // child
    int row1, row2, col1, col2;
    PQ temp;
    temp = heapp[0];
    row1 = temp.row;
    col1 = temp.col;
    while(j < size){
        if((j < (size-1)) && (heapp[j].distance > heapp[j+1].distance)) // if j is not the last one in heap
            j = j + 1;
        if(temp.distance <= heapp[j].distance)
            break;
        else{
            row2 = heapp[j].row;
            col2 = heapp[j].col;
            heapp[i] = heapp[j];
            board[row2*bcol + col2].heap_index = i;
            i = j;
            j = 2*i + 1;
        }
    }
    board[row1*bcol + col1].heap_index = i;
    heapp[i] = temp;
    return;
}

void UPWARD_HEAPIFY(PQ *heapp, Node *board, int bcol, int index){
    int row1 = heapp[index].row;
    int col1 = heapp[index].col;
    PQ temp = heapp[index];
    int row2;
    int col2;
    int i, j;
    i = index; // child
    j = (i-1)/2; // parent
    while((i > 0) && (heapp[j].distance > temp.distance)){
        row2 = heapp[j].row;
        col2 = heapp[j].col;
        heapp[i] = heapp[j];
        board[(row2 * bcol) + col2].heap_index = i;
        i = j;
        j = (i-1)/2;
    }
    heapp[i] = temp;
    board[(row1*bcol) + col1].heap_index = i;
    return;
}