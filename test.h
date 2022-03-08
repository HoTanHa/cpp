#include <stdio.h>

struct abc
{
    int a;
    int b;
    int c;
};

static struct abc abc_array[] = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8},
    {9, 10, 11},
    {12, 13, 14},
};

struct test
{
    struct abc *array_abc;
    int asss;
};

static struct abc a[] = {{1, 1, 1}};
static struct abc b[] = {
    {1, 1, 1},
    {2, 2, 2},
};
struct test abc_tesst = {
    .array_abc = abc_array,
    .asss = 10,
};