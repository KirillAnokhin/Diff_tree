#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include "Diff.h"
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
node_t* node_create(int type, float value, node_t* left, node_t* right);
node_t* node_copy(node_t* n);
node_t* diff(const node_t* n, FILE * out);
node_t* mult_diff(const node_t* n, FILE* out);
node_t* div_diff(const node_t* n, FILE* out);
node_t* pow_diff(node_t* n);
int tree_delete(node_t* n);
int tree_opt_rec(node_t* n, int* k);
int tree_opt(node_t* n);
int TreePrintTex(node_t* n, FILE* out);
int Tex_rec(node_t* n, FILE* out);
int Tex_init(FILE* out);
int Tex_end(FILE* out);
int Get_pdf();
node_t* GetG(const char * str);
node_t* GetN();
node_t* GetE();
node_t* GetT();
node_t* GetP();
node_t* GetV();
node_t* GetR();
node_t* GetD();
#endif
