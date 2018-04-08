#include <stdio.h>
#include <stdlib.h>
#include "diff_tree.h"
#include <assert.h>
#include <math.h>
int p = 0;
char* s = NULL;

node_t* GetG(const char * str)
{
    s = str;
    p = 0;
    node_t* tree = GetE();
    assert(s[p] == '\0');
    return tree;
}

node_t* GetE()
{
    node_t* val = GetT();
    node_t* tree = val;
    node_t* tmp;
    while(s[p] == '+' || s[p] == '-')
    {
        tmp = node_copy(tree);
        tree_delete(tree);
        char op = s[p];
        p++;
        node_t* val2 = GetT();
        if(op == '+')
            tree = _PLUS(tmp, val2);
        else
            tree = _MINUS(tmp, val2);
    }
    return tree;
}

node_t* GetT()
{
    node_t* val = GetD();
    node_t* tree = val;
    node_t* tmp;
    while(s[p] == '*' || s[p] == '/')
    {
        tmp = node_copy(tree);
        tree_delete(tree);
        char op = s[p];
        p++;
        node_t* val2 = GetD();
        if(op == '*')
            tree = _MULT(tmp, val2);
        else
            tree = _DIV(tmp, val2);
    }
    return tree;
}

node_t* GetD()
{
    node_t* val = GetP();
    node_t* tree = val;
    node_t* tmp;
    while(s[p] == '^')
    {
        tmp = node_copy(tree);
        tree_delete(tree);
        p++;
        node_t* val2 = GetP();
        tree = _POW(tmp, val2);
    }
    return tree;
}

node_t* GetP()
{
    if(s[p] == '(')
    {
        p++;
        node_t* val = GetE();
        assert(s[p] == ')');
        p++;
        return val;
    }
    else if(('a' <= s[p] && s[p] <= 'z') && !('a' <= s[p + 1] && s[p + 1] <= 'z'))
        return GetV();
    else if(('a' <= s[p] && s[p] <= 'z') && ('a' <= s[p + 1] && s[p + 1] <= 'z'))
        return GetR();
    else
        return GetN();
}

node_t* GetN()
{
    float val = 0;
    float rat = 0;
    int counter = 0;
    int counter1 = 1;
    while('0' <= s[p] && s[p] <= '9')
    {
        val = val * 10 + s[p] - '0';
        p++;
        counter++;
    }
    if(s[p] == '.')
    {
        p++;
        counter1 = 0;
        while('0' <= s[p] && s[p] <= '9')
        {
            rat = rat * 10 + s[p] - '0';
            p++;
            counter1++;
        }
    }
    printf("%f\n", val + rat / pow(10, counter1));
    if(!counter || !counter1)
    {
        printf("Error GetN %s\n", s + p);

        return 0;
    }
    node_t* tree = _NUMBER(val + rat / pow(10, counter1));
    return tree;
}

node_t* GetV()
{
    float val = 0;
    if(('a' <= s[p] && s[p] <= 'z') && !('a' <= s[p + 1] && s[p + 1] <= 'z'))
    {
        val = s[p];
        p++;
    }
    else
        printf("error %s\n", s + p);
    return _VAR(val);
}

node_t* GetR()
{
    if(s[p] == 's' && s[p + 1] == 'i' && s[p + 2] == 'n')
    {
        p += 3;
        node_t* val = _SIN(GetP());
        return val;
    }
    else if(s[p] == 'c' && s[p + 1] == 'o' && s[p + 2] == 's')
    {
        p += 3;
        node_t* val = _COS(GetP());
        return val;
    }
    else if(s[p] == 'l' && s[p + 1] == 'n')
    {
        p += 2;
        node_t* val = _LN(GetP());
    }
    else if(s[p] == 'c' && s[p + 1] == 'h')
    {
        p += 2;
        node_t* val = _CH(GetP());
        return val;
    }
    else if(s[p] = 's' && s[p + 1] == 'h')
    {
        p += 2;
        node_t* val = _SH(GetP());
        return val;
    }
    else if(s[p] = 't' && s[p + 1] == 'g')
    {
        p += 2;
        node_t* val = _TAN(GetP());
        return val;
    }
    else
    {
        printf("Invalid operator\n");
    }
}
