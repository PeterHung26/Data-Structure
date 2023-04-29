#ifndef __VHNODE__
#define __VHNODE__

typedef struct _Node {
   int weight;
   int short_weight;
   int pred_row;
   int pred_col;
   char pred;
   int heap_index;
} Node;

typedef struct _PQ {
   int row;
   int col;
   int distance;
} PQ;

#endif
