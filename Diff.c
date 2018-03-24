#include <assert.h>
#include "diff_tree.h"
#include "Diff.h"

char* get_op_str(float op)
{
    #define OPERATOR(name, symb) case OP_##name: return #symb;
    switch((int)op)
    {
        #include "alg.h"

    }
    assert(!"Unknown operator str");
    #undef OPERATOR
    return NULL;
}

char* get_type_str(int type)
{
    #define TYPE(name) case TYPE_##name: return #name;
    switch((int)type)
    {
        #include "types.h"
    }
    assert(!"Unknown type str");
    #undef TYPE
    return NULL;
}

int main()
{

    // CREATE TREE 5x + sinx;
    /*
    node_t* tree_l_l = node_create(TYPE_NUM, 5, NULL, NULL);
    node_t* tree_l_r = node_create(TYPE_VAR, 0, NULL, NULL);
    node_t* tree_r_l = node_create(TYPE_VAR, 0, NULL, NULL);
    node_t* tree_l   = node_create(TYPE_OP, OP_MULT, tree_l_l, tree_l_r);
   //node_t* tree_l   = node_create(TYPE_OP, OP_MULT, NULL, NULL);
    node_t* tree_r   = node_create(TYPE_OP, OP_SIN, tree_r_l, NULL);
    //node_t* tree_r   = node_create(TYPE_OP, OP_SIN, NULL, NULL);
    node_t* tree     = node_create(TYPE_OP, OP_PLUS, tree_l, tree_r);
    //node_t* tree     = node_create(TYPE_OP, OP_PLUS, NULL, NULL);
    node_t* d = diff(tree);
    Tree_Print(d);
    //node_t* tree_cop = node_copy(tree);
    //Tree_Print(tree_cop);
    ///get_op_str(tree_l -> value);
    //printf("%s", (get_type_str(tree -> type)));
   // diff(tree);
    */

    //  CREATE TREE 1/5x
   // node_t* tree_r_l = node_create(TYPE_NUM, 5, NULL, NULL);
   // node_t* tree_r_r = node_create(TYPE_VAR, 0, NULL, NULL);
   /*
    char x;
    node_t* tree_l = node_create(TYPE_NUM, 1, NULL, NULL);
    node_t* tree_r = node_create(TYPE_VAR, x, NULL, NULL);
    node_t* tree   = _DIV(tree_l, tree_r);
    node_t* d = diff(tree);
    Tree_Print(d);
    */
    //CREATE TREE COSx;
    /*
    node_t* tree_l = node_create(TYPE_VAR, 0, NULL, NULL);
    node_t* tree = node_create(TYPE_OP, OP_COS, tree_l, NULL);
    node_t* d = diff(tree);
    Tree_Print(d);
    */
    //CREATE TREE shx
    /*
    node_t* tree_l = node_create(TYPE_VAR, 'x', NULL, NULL);
    node_t* tree = node_create(TYPE_OP, OP_SH, tree_l, NULL);
    node_t* d = diff(tree);
    Tree_Print(d);
    */
    //CREATE TREE x^2
    /*
    node_t* tree_l = node_create(TYPE_VAR, 'x', NULL, NULL);
    node_t* tree_r = node_create(TYPE_VAR, 'x', NULL, NULL);

    node_t* tree = node_create(TYPE_OP, OP_POW, tree_l, tree_r);

    node_t* d = diff(tree);

    Tree_Print(d);
    */
    //CREATE TREE tg(x)
    node_t* tree_l = node_create(TYPE_VAR, 'x', NULL, NULL);
    node_t* tree = node_create(TYPE_OP, OP_TAN, tree_l, NULL);
    node_t* d = diff(tree);
    Tree_Print(d);
    return 0;
}
