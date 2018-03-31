#include <assert.h>
#include "diff_tree.h"
#include "Diff.h"

//char* get_op_str(float op);
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
    node_t* tree_l_r = node_create(TYPE_VAR, 'x', NULL, NULL);
    node_t* tree_r_l = node_create(TYPE_VAR, 'x', NULL, NULL);
    node_t* tree_l   = node_create(TYPE_OP, OP_MULT, tree_l_l, tree_l_r);
    node_t* tree_r   = node_create(TYPE_OP, OP_SIN, tree_r_l, NULL);
    node_t* tree     = node_create(TYPE_OP, OP_PLUS, tree_l, tree_r);
    node_t* d = diff(tree);
    tree_opt(d);
    Tree_Print(d);

    tree_delete(d);
    tree_delete(tree);
    */



    //  CREATE TREE 1/x
    /*
    node_t* tree_l = node_create(TYPE_NUM, 1, NULL, NULL);
    node_t* tree_r = node_create(TYPE_VAR, 'x', NULL, NULL);
    node_t* tree   = _DIV(tree_l, tree_r);
    node_t* d = diff(tree);
    tree_opt(d);
    Tree_Print(d);

    tree_delete(d);
    tree_delete(tree);
    */


    //CREATE TREE cos(x) + sh(x + 5) * chx;
    /*
    node_t* tree_r = _MULT(_SH(_PLUS(node_create(TYPE_VAR, 'x', NULL, NULL), node_create(TYPE_NUM, 5, NULL, NULL))), _SH(node_create(TYPE_VAR, 'x', NULL, NULL)));
    node_t* tree_l = _COS(node_create(TYPE_VAR, 'x', NULL, NULL));
    node_t* tree   = _PLUS(tree_l, tree_r);
    node_t* d = diff(tree);
    tree_opt(d);
    Tree_Print(d);

    tree_delete(d);
    tree_delete(tree);
    */

    //CREATE TREE x^x
    /*
    node_t* tree_l = node_create(TYPE_VAR, 'x', NULL, NULL);
    node_t* tree_r = node_create(TYPE_VAR, 'x', NULL, NULL);

    node_t* tree = node_create(TYPE_OP, OP_POW, tree_l, tree_r);

    node_t* d = diff(tree);
    tree_opt(d);
    Tree_Print(d);

    tree_delete(d);
    tree_delete(tree);
    */


    //CREATE TREE tg(x)
    /*
    node_t* tree_l = node_create(TYPE_VAR, 'x', NULL, NULL);
    node_t* tree = node_create(TYPE_OP, OP_TAN, tree_l, NULL);
    node_t* d = diff(tree);
    tree_opt(d);
    Tree_Print(d);

    tree_delete(d);
    tree_delete(tree);
    */

    //CREATE TREE 2^(ln(x + 6))
    /*
    node_t* tree_r = _LN(_PLUS(node_create(TYPE_VAR, 'x', NULL, NULL), node_create(TYPE_NUM, 6, NULL, NULL)));
    node_t* tree   = _POW(node_create(TYPE_NUM, 2, NULL, NULL), tree_r);
    node_t* d = diff(tree);
    tree_opt(d);
    Tree_Print(d);

    tree_delete(d);
    tree_delete(tree);
    */

    // CREATE TREE tg(x^2 + 8) / ln(sinx * cosx)
    /*
    node_t* numerator = _TAN(_PLUS(_POW(node_create(TYPE_VAR, 'x', NULL, NULL), node_create(TYPE_VAR, 'x', NULL, NULL)), node_create(TYPE_NUM, 8, NULL, NULL)));
    node_t* denominator = _LN(_MULT(_SIN(node_create(TYPE_VAR, 'x', NULL, NULL)),_COS(node_create(TYPE_VAR, 'x', NULL, NULL))));
    node_t* tree = _DIV(numerator, denominator);
    node_t* d = diff(tree);
    tree_opt(d);
    Tree_Print(d);

    tree_delete(d);
    tree_delete(tree);
    */

    //create tree MAIN TEST
    /*
    node_t* sinus = _MULT(_SIN(node_create(TYPE_VAR, 'a', NULL, NULL)), _SIN(node_create(TYPE_VAR, 'a', NULL, NULL)));
    node_t* denominator = _COS(_PLUS(sinus, _MULT(node_create(TYPE_NUM, 4, NULL, NULL), node_create(TYPE_VAR, 'x', NULL, NULL))));
    node_t* log = _LN(_MINUS(_MULT(node_create(TYPE_VAR, 'x', NULL, NULL), node_create(TYPE_VAR, 'x', NULL, NULL)), node_create(TYPE_NUM, 4, NULL, NULL)));
    node_t* power = _PLUS(_SIN(node_create(TYPE_VAR, 'x', NULL, NULL)), log);
    node_t* numerator = _POW(node_create(TYPE_VAR, 'x', NULL, NULL), power);
    node_t* tree = _DIV(numerator, denominator);

    node_t* d = diff(tree);
    tree_opt(d);
    Tree_Print(d);

    tree_delete(tree);
    tree_delete(tree);
    */

    FILE* result;
    result = fopen("res.tex", "w");
    Tex_init(result);

    node_t* sinus = _MULT(_SIN(node_create(TYPE_VAR, 'a', NULL, NULL)), _SIN(node_create(TYPE_VAR, 'a', NULL, NULL)));
    node_t* denominator = _COS(_PLUS(sinus, _MULT(node_create(TYPE_NUM, 4, NULL, NULL), node_create(TYPE_VAR, 'x', NULL, NULL))));
    node_t* log = _LN(_MINUS(_MULT(node_create(TYPE_VAR, 'x', NULL, NULL), node_create(TYPE_VAR, 'x', NULL, NULL)), node_create(TYPE_NUM, 4, NULL, NULL)));
    node_t* power = _PLUS(_SIN(node_create(TYPE_VAR, 'x', NULL, NULL)), log);
    node_t* numerator = _POW(node_create(TYPE_VAR, 'x', NULL, NULL), power);
    node_t* tree = _DIV(numerator, denominator);


    node_t* d = diff(tree);
    tree_opt(d);
    TreePrintTex(d, result);
    Tex_end(result);
    fclose(result);
    Get_pdf();
    tree_delete(tree);
    tree_delete(d);

    return 0;
}
