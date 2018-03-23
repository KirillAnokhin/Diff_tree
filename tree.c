#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "diff_tree.h"
#include "Diff.h"

node_t* node_create(int type, float value, node_t* left, node_t* right)
{
    node_t* n = (node_t*)calloc(1, sizeof(node_t));
    assert(n);
    n -> left = left;
    n -> right = right;
    n -> type = type;
    n -> value = value;
    return n;
}

node_t* node_copy(node_t* n)
{
    node_t* r = (node_t*)calloc(1, sizeof(node_t));
    assert(n);
    r -> type = n -> type;
    r -> value = n -> value;
    if(n -> left != NULL)
        r -> left = node_copy(n -> left);
    if(n -> right != NULL)
        r -> right = node_copy(n -> right);
    return r;
}



node_t* diff(const node_t* n)
{
    assert(n);
    switch(n -> type)
    {
        case TYPE_NUM:
            return _ZERO;
        case TYPE_VAR:
            return _ONE;
        case TYPE_OP:
            switch((int)(n -> value))
            {
                case OP_PLUS:
                    return _PLUS(diff(n -> left), diff(n -> right));
                case OP_MINUS:
                    return _MINUS(diff(n -> left), diff(n -> right));
                case OP_MULT:
                    return mult_diff(n);
                case OP_DIV:
                    return div_diff(n);
                case OP_SIN:
                    return _MULT(diff(n -> left), _COS(n -> left));
                case OP_COS:
                    return _MULT(node_create(TYPE_NUM, -1, NULL, NULL), _MULT(diff(n->left), _SIN(n -> left)));
                case OP_POW:
                {
                    if(n -> left -> type == TYPE_VAR && n -> right -> type == TYPE_NUM)
                        return _MULT(node_create(TYPE_NUM, n -> right -> value, NULL, NULL), _POW(node_create(TYPE_VAR, var_x, NULL, NULL),
                        node_create(TYPE_NUM, n -> right -> type - 1, NULL, NULL)));
                }
            }

    }
    assert(0);
    return NULL;
}

node_t* mult_diff(node_t* n)
{
        node_t* u_ = diff(n -> left);
        node_t* v  = node_copy(n -> right);
        node_t* v_ = diff(n -> right);
        node_t* u  = node_copy(n -> left);
        return _PLUS(_MULT(u_, v), _MULT(u, v_));
}

node_t* div_diff(node_t* n)
{
    node_t* u_ = diff(n -> left);
    node_t* v  = node_copy(n -> right);
    node_t* v_ = diff(n -> right);
    node_t* u  = node_copy(n -> left);
    node_t* znam1 = node_copy(v);
    node_t* znam2 = node_copy(v);
    node_t* numerator   = _MINUS(_MULT(u_, v), _MULT(u, v_));
    node_t* denominator = _MULT(znam1, znam2);
    return _DIV(numerator, denominator);
}

int Tree_Print(node_t* n)
{
    FILE* output = fopen("dump.dot", "w");
    fprintf(output, "digraph dump {\nnode [shape = record];\n");
    TreePrintRec(n, output);
    fprintf(output, "}");
    fclose(output);
    system("dot -Tps dump.dot -o dumping.ps");
    system("xdg-open dumping.ps");
    return 0;
}

int TreePrintRec(node_t* n, FILE* output)
{
    if(n -> type == TYPE_VAR)
        fprintf(output, "%ld [shape = record, label = \"{ptr: %ld|type: %s|value: %c|left: %ld|right: %ld}\"];",
        (long int)n, (long int)n, get_type_str(n->type), (int)n -> value, (long int)n->left, (long int)n->right);
    else if(n -> type == TYPE_OP)
    {
        fprintf(output, "%ld [shape = record, label = \"{ptr: %ld|type: %s|value: %s|left: %ld|right: %ld}\"];",
        (long int)n, (long int)n, get_type_str(n->type), get_op_str(n->value), (long int)n->left,(long int)n->right);
    }
    else
        fprintf(output, "%ld [shape = record, label = \"{ptr: %ld|type: %s|value: %f|left: %ld|right: %ld}\"];",
        (long int)n, (long int)n, get_type_str(n->type), n->value, (long int)n->left, (long int)n->right);
    if(n -> left != NULL)
    {
        fprintf(output, "%ld -> %ld\n", (long int)n, (long int)n->left);
        TreePrintRec(n -> left, output);
    }
    if(n -> right != NULL)
    {
        fprintf(output, "%ld -> %ld\n", (long int)n, (long int)n->right);
        TreePrintRec(n -> right, output);
    }
    return 0;
}


