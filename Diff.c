#include <assert.h>
#include "diff_tree.h"
#include "Diff.h"
#define BUF_SIZE 100

char* get_op_str(float op);
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



    FILE* result;
    result = fopen("res.tex", "w");
    Tex_init(result);

    char buf[BUF_SIZE] = {};
    FILE* stream = fopen("input.txt", "r");
    fscanf(stream, "%s", buf);
    node_t* tree = GetG(buf);

    fprintf(result, "Исходное выражение:\n");
    TreePrintTex(tree, result);
    node_t* d = diff(tree, result);
    tree_opt(d);
    fprintf(result, "Производная равна:\n");
    TreePrintTex(d, result);
    Tex_end(result);

    fclose(result);
    fclose(stream);
    Get_pdf();
    tree_delete(tree);
    tree_delete(d);

    return 0;
}
