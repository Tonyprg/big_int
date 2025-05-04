#ifndef BIG_INT_H
#define BIG_INT_H

#include <stdio.h>

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(a)    MAX(-(a), (a))
#endif

typedef struct BigInt
{
    int     sign;
    size_t  len;
    int    *data;
} BigInt;

BigInt* big_int_new          (int n, size_t len);
BigInt* big_int_copy         (BigInt *bn);
char*   big_int_to_cstr      (BigInt *bn);
void    print_big_int        (BigInt *bn);
void    println_big_int      (BigInt *bn);
void    big_int_free         (BigInt *bn);
int     big_int_positive_cmp (BigInt *a, BigInt *b);
int     big_int_cmp          (BigInt *a, BigInt *b);
BigInt* big_int_positive_sum (BigInt *a, BigInt *b);
BigInt* big_int_positive_sub (BigInt *a, BigInt *b);
BigInt* big_int_mul          (BigInt *a, BigInt *b);

void test ();

#endif
