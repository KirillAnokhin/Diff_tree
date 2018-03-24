#ifndef _Diff_h_
#define _Diff_h_


#define _PLUS(node_pointer_1, node_pointer_2) node_create(TYPE_OP, OP_PLUS, node_pointer_1, node_pointer_2)
#define _MINUS(node_pointer_1, node_pointer_2) node_create(TYPE_OP, OP_MINUS, node_pointer_1, node_pointer_2)
#define _MULT(node_pointer_1, node_pointer_2) node_create(TYPE_OP, OP_MULT, node_pointer_1, node_pointer_2)
#define _ZERO node_create(TYPE_NUM, 0, NULL, NULL)
#define _ONE node_create(TYPE_NUM, 1, NULL, NULL)
#define _SIN(node_pointer) node_create(TYPE_OP, OP_SIN, node_pointer, NULL)
#define _COS(node_pointer) node_create(TYPE_OP, OP_COS, node_pointer, NULL)
#define _DIV(node_pointer_1, node_pointer_2) node_create(TYPE_OP, OP_DIV, node_pointer_1, node_pointer_2)
#define _POW(node_pointer_1, node_pointer_2) node_create(TYPE_OP, OP_POW, node_pointer_1, node_pointer_2)
#define _LN(node_pointer_1) node_create(TYPE_OP, OP_LN, node_pointer_1, NULL)
#define _SH(node_pointer) node_create(TYPE_OP, OP_SH, node_pointer, NULL)
#define _CH(node_pointer) node_create(TYPE_OP, OP_CH, node_pointer, NULL)
#define _TAN(node_pointer) node_create(TYPE_OP, OP_TAN, node_pointer, NULL)

enum ALG_CODES  //OP_PLUS = 1 etc
{
    #define OPERATOR(name, symb) OP_##name,

    CODE_NULL = 0,
    #include "alg.h"

    #undef OPERATOR
};

enum TYPES  //TYPE_OP, TYPE_NUM, TYPE_VAR
{
    #define TYPE(name) TYPE_##name,

    TYPE_NULL = 0,
    #include "types.h"

    #undef TYPE
};
char* get_op_str(float op);
char* get_type_str(int type);
#endif
