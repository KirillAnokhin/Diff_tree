#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct node_t node_t;
struct node_t
{
    int type;
    float value;
    struct node_t *left;
    struct node_t *right;
};
int Tree_Print(node_t* n);
int TreePrintRec(node_t* n, FILE* output);

//node_t* read_tree(char **cur);

node_t* node_create(int type, float value, node_t* left, node_t* right);
node_t* node_copy(node_t* n);
node_t* diff(const node_t* n);
node_t* mult_diff(node_t* n);
node_t* div_diff(node_t* n);
node_t* pow_diff(node_t* n);

#endif
