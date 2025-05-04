#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "big_int.h"

int BIG_INT_BASE = 10;

static int number_len (int n)
{
    n = ABS(n);
    int res = 0;
    while (n != 0)
    {
        ++res;
        n /= 10;
    }
    return res;
}

BigInt* big_int_new (int n, size_t len)
{
    len = MAX(len, number_len(n));

    BigInt *res = (BigInt*)malloc(sizeof(BigInt));

    if (n > 0) res->sign = 1;
    else if (n == 0) res->sign = 0;
    else
    {
        res->sign = -1;
        n *= -1;
    }

    res->len = 0;

    res->data = (int*)malloc(sizeof(int) * len);
    while (n != 0)
    {
        res->data[(res->len)++] = n % BIG_INT_BASE;
        n /= BIG_INT_BASE;
    }

    // Заполняем нулями оставшееся место,
    // расширяем переменную res->len до len в случае необходимости
    while (res->len < len)
        res->data[(res->len)++] = 0;

    return res;
}

BigInt* big_int_copy (BigInt *bn)
{
    BigInt *res = (BigInt*)malloc(sizeof(BigInt));
    res->sign = bn->sign;
    res->len = bn->len;
    res->data = (int*)malloc(sizeof(int) * res->len);
    for (size_t i = 0; i < res->len; ++i)
        res->data[i] = bn->data[i];
    return res;
}

char* big_int_to_cstr (BigInt *bn)
{
    size_t cap = 1;
    size_t len = 0;
    char *s = (char*)malloc(cap + 1);

    if (bn->sign == -1)
    {
        if (len >= cap)
            s = (char*)realloc(s, (cap *= 2) + 1);
        s[len++] = '-';
    }

    for (size_t i = bn->len - 1; i < bn->len; --i)
    {
        if (bn->data[i] == 0)
            continue;

        for (size_t j = i; j < bn->len; --j)
        {
            if (len >= cap)
                s = (char*)realloc(s, (cap *= 2) + 1);
            s[len++] = bn->data[j] + '0';
        }

        s[len] = '\0';
        s = realloc(s, len + 1);
        return s;
    }

    free(s);
    return strdup("0");
}

void print_big_int (BigInt *bn)
{
    char *s = big_int_to_cstr(bn);
    printf("%s", s);
    free(s);
}

void println_big_int (BigInt *bn)
{
    print_big_int(bn);
    printf("\n");
}

void big_int_free (BigInt *bn)
{
    if (bn == NULL) return;
    free(bn->data);
    free(bn);
}

int big_int_positive_cmp (BigInt *a, BigInt *b)
{
    size_t a_end = a->len;
    while (a_end > 0 && a->data[a_end - 1] == 0)
        --a_end;

    size_t b_end = b->len;
    while (b_end > 0 && b->data[b_end - 1] == 0)
        --b_end;

    if (a_end < b_end)
        return -1;
    else if (a_end > b_end)
        return 1;
    else
        for (size_t i = a_end - 1; i < a_end; --i)
            if (a->data[i] < b->data[i])
                return -1;
            else if (a->data[i] > b->data[i])
                return 1;

    return 0;
}

int big_int_cmp (BigInt *a, BigInt *b)
{
    return a->sign == 0       ? -b->sign :
           b->sign == 0       ? a->sign :
           a->sign != b->sign ? ((a->sign < b->sign) ? -1 : 1) :
           a->sign > 0        ? big_int_positive_cmp(a, b) :
                               -big_int_positive_cmp(a, b);
}

BigInt* big_int_positive_sum (BigInt *a, BigInt *b)
{
    if (a->sign == 0)
        return big_int_copy(b);
    else if (b->sign == 0)
        return big_int_copy(a);

    BigInt *c = big_int_new(0, MAX(a->len, b->len) + 1);
    c->sign = 1;

    for (size_t i = 0; i + 1 < c->len; ++i)
    {
        int a_digit = i < a->len ? a->data[i] : 0;
        int b_digit = i < b->len ? b->data[i] : 0;
        c->data[i] += a_digit + b_digit;
        c->data[i + 1] += c->data[i] / BIG_INT_BASE;
        c->data[i] %= BIG_INT_BASE;
    }

    return c;
}

BigInt* big_int_positive_sub (BigInt *a, BigInt *b)
{
    BigInt *c = big_int_copy(a);
    for (size_t i = 0; i < b->len; ++i)
    {
        c->data[i] -= b->data[i];
        if (c->data[i] < 0)
        {
            c->data[i] += BIG_INT_BASE;
            c->data[i + 1] -= 1;
        }
    }
    return c;
}

// Необходимо протестировать
BigInt* big_int_mul (BigInt *a, BigInt *b)
{
    if (a->sign == 0 || b->sign == 0)
        return big_int_new(0, 0);

    BigInt *c = big_int_new(0, a->len + b->len);

    c->sign = a->sign * b->sign;
    for (size_t i = 0; i < a->len; ++i)
        for (size_t j = 0; j < b->len; ++j)
            c->data[i + j] += a->data[i] * b->data[j];

    for (size_t i = 0; i + 1 < c->len; ++i)
    {
        c->data[i + 1] += c->data[i] / BIG_INT_BASE;
        c->data[i] %= BIG_INT_BASE;
    }

    return c;
}

// Необходимо дописать реализацию
BigInt* big_int_div (BigInt *a, BigInt *b)
{
    BigInt *c = big_int_new(0, a->len - b->len + 1);
}
