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
                    if(n -> left -> type == TYPE_NUM || (n -> left -> type == TYPE_VAR && n -> left -> value != 'x'))
                        return _ZERO;
                    else
                        return _MULT(diff(n -> left), _COS(n -> left));

                case OP_COS:
                    if(n -> left -> type == TYPE_NUM || (n -> left -> type == TYPE_VAR && n -> left -> value != 'x'))
                        return _ZERO;
                    else
                        return _MULT(node_create(TYPE_NUM, -1, NULL, NULL), _MULT(diff(n->left), _SIN(n -> left)));

                case OP_POW:
                {
                    if(n -> right -> type == TYPE_NUM) // ((f)^a)' = a * (f)^(a - 1) * f' = a * (f)^b * f'
                    {
                        node_t* a = node_create(TYPE_NUM, n -> right -> value, NULL, NULL);
                        node_t* b = node_create(TYPE_NUM, n -> right -> value - 1, NULL, NULL);
                        node_t* f_ = diff(n -> left);
                        node_t* f = node_copy(n -> left);
                        return _MULT(_MULT(a, _POW(f, b)), f_);
                    }
                    else if(n -> left -> type == TYPE_NUM) // (a^(f))' = ln(a) * a^(f) * f'
                    {
                        node_t* a = node_create(TYPE_NUM, n -> left -> value, NULL, NULL);
                        node_t* ln_a = _LN(a);
                        node_t* f_ = diff(n -> right);
                        node_t* f = node_copy(n -> right);
                        return _MULT(_MULT(ln_a, _POW(node_copy(a), f)), f_);
                    }
                    else // (f^(g))' = (e^(g*ln(f)))' = e^(g * ln(f)) * (g * ln(f))'
                    {
                        node_t* f = node_copy(n -> left);
                        node_t* g = node_copy(n -> right);
                        node_t* e = node_create(TYPE_VAR, 'e', NULL, NULL);
                        node_t* ln_f = _LN(node_copy(f));
                        return _MULT(_POW(e, _MULT(g, ln_f)), mult_diff(_MULT(node_copy(g), node_copy(ln_f))));
                    }
                }

                case OP_LN:
                    if(n -> left -> type == TYPE_NUM || (n -> left -> type == TYPE_VAR && n -> left -> value != 'x'))
                        return _ZERO;
                    else
                        return _DIV(diff(n -> left), node_copy(n -> left));

                case OP_SH:
                    if(n -> left -> type == TYPE_NUM || (n -> left -> type == TYPE_VAR && n -> left -> value != 'x'))
                        return _ZERO;
                    else
                        return _MULT(diff(n -> left), _CH(n -> left));

                case OP_CH:
                    if(n -> left -> type == TYPE_NUM || (n -> left -> type == TYPE_VAR && n -> left -> value != 'x'))
                        return _ZERO;
                    else
                        return _MULT(diff(n -> left), _SH(n -> left));

                case OP_TAN:
                    if(n -> left -> type == TYPE_NUM || (n -> left -> type == TYPE_VAR && n -> left -> value != 'x'))
                        return _ZERO;
                    else
                        return _DIV(diff(n -> left), _MULT(node_copy(_COS(n -> left)), node_copy(_COS(n -> left))));
            }

    }
    assert(0);
    return NULL;
}

node_t* mult_diff(const node_t* n)
{
        node_t* u_ = diff(n -> left);
        node_t* v  = node_copy(n -> right);
        node_t* v_ = diff(n -> right);
        node_t* u  = node_copy(n -> left);
        return _PLUS(_MULT(u_, v), _MULT(u, v_));
}

node_t* div_diff(const node_t* n)
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

int tree_delete(node_t* n)
{
    if(n -> left != NULL)
        tree_delete(n -> left);
    if(n -> right != NULL)
        tree_delete(n -> right);
    free(n);
    return 0;
}

int tree_opt_rec(node_t* n, int* k)
{
    if(n -> type == TYPE_OP)
    {
        switch((int)n -> value)
        {
            case OP_PLUS:
                if(n -> left -> type == TYPE_NUM && n -> right -> type == TYPE_NUM)
                {
                    n -> type = TYPE_NUM;
                    n -> value = (n -> left -> value) + (n -> right -> value);
                    tree_delete(n -> left);
                    tree_delete(n -> right);
                    n -> left = NULL;
                    n -> right = NULL;
                    (*k)++;
                }
                else if(n -> left -> type == TYPE_NUM && n -> left -> value == 0)
                {
                    n -> type = n -> right -> type;
                    n -> value = n -> right -> value;
                    node_t * tmp = n -> right;
                    n -> right = tmp -> right;
                    n -> left = tmp -> left;
                    free(tmp);
                    (*k)++;
                }
                else if(n -> right -> type == TYPE_NUM && n -> right -> value == 0)
                {
                    n -> type = n -> left -> type;
                    n -> value = n -> left -> value;
                    node_t * tmp = n -> left;
                    n -> right = tmp -> right;
                    n -> left = tmp -> left;
                    free(tmp);
                    (*k)++;
                }

                break;
            case OP_MULT:
                if((n -> left -> type == TYPE_NUM) && (n -> right -> type == TYPE_NUM))
                {
                    n -> type = TYPE_NUM;
                    n -> value = (n -> left -> value) * (n -> right -> value);
                    tree_delete(n -> left);
                    tree_delete(n -> right);
                    n -> left = NULL;
                    n -> right = NULL;
                    (*k)++;
                }
                else if(n -> right -> type == TYPE_NUM && n -> right -> value == 0)
                {
                    n -> type = TYPE_NUM;
                    n -> value = 0;
                    tree_delete(n -> left);
                    tree_delete(n -> right);
                    n -> left = NULL;
                    n -> right = NULL;
                    (*k)++;
                }
                else if(n -> left -> type == TYPE_NUM && n -> left -> value == 0)
                {
                    n -> type = TYPE_NUM;
                    n -> value = 0;
                    tree_delete(n -> left);
                    tree_delete(n -> right);
                    n -> left = NULL;
                    n -> right = NULL;
                    (*k)++;
                }
                else if(n -> right -> type == TYPE_NUM && n -> right -> value == 1)
                {
                    n -> type = n -> left -> type;
                    n -> value = n -> left -> value;
                    node_t * tmp = n -> left;
                    n -> left = tmp -> left;
                    n -> right = tmp -> right;
                    free(tmp);
                    (*k)++;
                }
                else if(n -> left -> type == TYPE_NUM && n -> left -> value == 1)
                {
                    n -> type = n -> right -> type;
                    n -> value = n -> right -> value;
                    node_t * tmp = n -> right;
                    n -> left = tmp -> left;
                    n -> right = tmp -> right;
                    free(tmp);
                    (*k)++;
                }
                break;
            case OP_MINUS:
                if(n -> left -> type == TYPE_NUM && n -> right -> type == TYPE_NUM)
                {
                    n -> type = TYPE_NUM;
                    n -> value = (n -> left -> value) - (n -> right -> value);
                    tree_delete(n -> left);
                    tree_delete(n -> right);
                    n -> left = NULL;
                    n -> right = NULL;
                    (*k)++;
                }

                else if(n -> right -> type == TYPE_NUM && n -> right -> value == 0)
                {
                    n -> value = OP_PLUS;
                    (*k)++;
                }
                break;
            case OP_DIV:

                if(n -> right -> type == TYPE_NUM && n -> right -> value == 0)
                {
                    assert(!" error denominator is zero ");
                }
                else
                {
                    if(n -> left -> type == TYPE_NUM && n -> right -> type == TYPE_NUM)
                    {
                        n -> type = TYPE_NUM;
                        n -> value = (n -> left -> value) / (n -> right -> value);
                        tree_delete(n -> left);
                        tree_delete(n -> right);
                        n -> left = NULL;
                        n -> right = NULL;
                        (*k)++;
                    }
                    else if(n -> left -> type == TYPE_NUM && n -> left -> value == 0)
                    {
                        n -> type = TYPE_NUM;
                        n -> value = 0;
                        tree_delete(n -> left);
                        tree_delete(n -> right);
                        n -> left = NULL;
                        n -> right = NULL;
                        (*k)++;
                    }
                    else if(n -> right -> type == TYPE_NUM && n -> right -> value == 1)
                    {
                        n -> type = n -> left -> type;
                        n -> value = n -> left -> value;
                        node_t * tmp = n -> left;
                        n -> left = tmp -> left;
                        n -> right = tmp -> right;
                        free(tmp);
                        (*k)++;
                    }
                }
                break;
        }
    }
    if(n -> left != NULL)
        tree_opt_rec(n -> left, k);
    if(n -> right != NULL)
        tree_opt_rec(n -> right, k);
    return 0;
}

int tree_opt(node_t* n)
{
    int p;
    p = 1;
    while(p != 0)
    {
        p = 0;
        tree_opt_rec(n, &p);
    }
    return 0;
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

int TreePrintTex(node_t* n, FILE* out)
{
    fprintf(out, " $$");
    Tex_rec(n, out);
    fprintf(out, "$$\n");
    return 0;
}

int Tex_rec(node_t* n, FILE* out)
{
    if(n -> left == NULL && n -> right == NULL)
        switch(n -> type)
        {
            case TYPE_NUM:
                fprintf(out, "%.2f", n->value);
                break;

            case TYPE_VAR:
                fprintf(out, "%c", (int)n->value);
                break;
        }
    else
    {
        if(n -> value == OP_PLUS)
        {
            Tex_rec(n -> left, out);
            fprintf(out, "+");
            Tex_rec(n -> right, out);
        }

        if(n -> value == OP_MINUS)
        {
            Tex_rec(n -> left, out);
            fprintf(out, "-");
            Tex_rec(n -> right, out);
        }

        if(n -> value == OP_MULT)
        {
            Tex_rec(n -> left, out);
            fprintf(out, " \\cdot ");
            Tex_rec(n -> right, out);
        }

        if(n -> value == OP_DIV)
        {
            fprintf(out, "\\frac{");
            Tex_rec(n -> left, out);
            fprintf(out, "}{");
            Tex_rec(n -> right, out);
            fprintf(out, "}");
        }

        if(n -> value == OP_POW)
        {
            Tex_rec(n -> left, out);
            fprintf(out, "^ {");
            Tex_rec(n -> right, out);
            fprintf(out, "}");
        }

        if(n -> value == OP_SIN)
        {
            fprintf(out, "sin(");
            Tex_rec(n -> left, out);
            fprintf(out, ")");
        }

        if(n -> value == OP_COS)
        {
            fprintf(out, "cos(");
            Tex_rec(n -> left, out);
            fprintf(out, ")");
        }

        if(n -> value == OP_LN)
        {
            fprintf(out, "ln(");
            Tex_rec(n -> left, out);
            fprintf(out, ")");
        }

        if(n -> value == OP_SH)
        {
            fprintf(out, "sh(");
            Tex_rec(n -> left, out);
            fprintf(out, ")");
        }

        if(n -> value == OP_CH)
        {
            fprintf(out, "ch(");
            Tex_rec(n -> left, out);
            fprintf(out, ")");
        }

        if(n -> value == OP_TAN)
        {
            fprintf(out, "tg(");
            Tex_rec(n -> left, out);
            fprintf(out, ")");
        }
    }
    return 0;
}

int Tex_init(FILE* out)
{
    assert(out);
    fprintf(out, "\\documentclass[12pt]{article}\n\
    \\usepackage{ucs}\n\
    \\usepackage[utf8x]{inputenc}\n\
    \\usepackage[english,russian]{babel}\n\
    \\pagestyle{empty}\n\
    \\usepackage{amsmath}\n\
    \n\\begin{document}\n");
    return 0;
}

int Tex_end(FILE* out)
{
    assert(out);
    fprintf(out, "\\end{document}\n");
    return 0;
}

int Get_pdf()
{
    system("pdflatex res.tex");
    system("xdg-open res.pdf");
    return 0;
}
