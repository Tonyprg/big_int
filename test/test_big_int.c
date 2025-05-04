#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "big_int.h"

static bool is_fail = false;

#define CHECK_COND(cond)                                           \
    do {                                                           \
        if ((cond) == false)                                       \
        {                                                          \
            is_fail = true;                                        \
            printf("Ошибка тестированияв %i строке.\n", __LINE__); \
        }                                                          \
    } while (0);

void test_big_new ()
{
    BigInt *n = NULL;

    n = big_int_new(0, 0);
    CHECK_COND(n->sign == 0);
    CHECK_COND(n->len == 0);
    big_int_free(n);

    n = big_int_new(12, 100);
    CHECK_COND(n->sign == 1);
    CHECK_COND(n->len == 100);
    CHECK_COND(n->data[0] == 2);
    CHECK_COND(n->data[1] == 1);
    for (size_t i = 2; i < 100; ++i)
        CHECK_COND(n->data[i] == 0);
    big_int_free(n);

    n = big_int_new(123456789, 8);
    CHECK_COND(n->sign == 1);
    CHECK_COND(n->len == 9);
    CHECK_COND(n->data[0] == 9);
    CHECK_COND(n->data[1] == 8);
    CHECK_COND(n->data[2] == 7);
    CHECK_COND(n->data[3] == 6);
    CHECK_COND(n->data[4] == 5);
    CHECK_COND(n->data[5] == 4);
    CHECK_COND(n->data[6] == 3);
    CHECK_COND(n->data[7] == 2);
    CHECK_COND(n->data[8] == 1);
    big_int_free(n);
}

void test_big_int_to_cstr ()
{

#define NUMS {   \
    X(0),        \
    X(3910),     \
    X(941),      \
    X(1),        \
    X(0),        \
    X(-0),       \
    X(94015),    \
    X(39),       \
    X(-9415),    \
    X(-39),      \
    X(940500),   \
    X(-39),      \
    X(910251),   \
}

#define X(n) n
    int input[] = NUMS;
#undef X

#define X(n) ((n) == 0 ? "0" : #n)
    char *output[] = NUMS;
#undef X

#undef NUMS

    for (size_t i = 0; i < sizeof(input) / sizeof(*input); ++i)
    {
        BigInt *bn = big_int_new(input[i], 0);
        char *s = big_int_to_cstr(bn);
        if (strcmp(s, output[i]) != 0)
        {
            printf("Не совпадют: %s и %s\n", output[i], s);
            is_fail = true;
        }
        free(s);
        big_int_free(bn);
    }
}

void test_big_int_cmp ()
{
#define LIST {     \
    X(0, 0),       \
    X(0, 1),       \
    X(1, 0),       \
    X(100, 200),   \
    X(13, 17),     \
    X(45, 23),     \
    X(999, 99999), \
    X(99, 999),    \
    X(9, 11),      \
}

#define X(a, b) {a, b}
    int input[][2] = LIST;
#undef X

#define X(a, b) (a - b <  0 ? -1 : \
                 a - b == 0 ?  0 : \
                               1)
    int check_cmp[] = LIST;
#undef X

    for (int i = 0; i < sizeof(check_cmp) / sizeof(*check_cmp); ++i)
    {
        BigInt *a = big_int_new(input[i][0], 0);
        BigInt *b = big_int_new(input[i][1], 0);
        CHECK_COND(big_int_cmp(a, b) == check_cmp[i]);
        big_int_free(a);
        big_int_free(b);
    }
#undef LIST
}

void test_big_int_positive_sum ()
{
#define LIST {     \
    X(0, 0),       \
    X(0, 1),       \
    X(1, 0),       \
    X(100, 200),   \
    X(13, 17),     \
    X(45, 23),     \
    X(999, 99999), \
    X(99, 999),    \
    X(9, 11),      \
}

#define X(a, b) {a, b}
    int input[][2] = LIST;
#undef X

#define X(a, b) ABS(a) + ABS(b)
    int check_sum[] = LIST;
#undef X

    for (int i = 0; i < sizeof(check_sum) / sizeof(*check_sum); ++i)
    {
        BigInt *a  = big_int_new(input[i][0], 0);
        BigInt *b  = big_int_new(input[i][1], 0);
        BigInt *ab = big_int_positive_sum(a, b);
        BigInt *c  = big_int_new(check_sum[i], 0);
        CHECK_COND(big_int_cmp(ab, c) == 0);
        big_int_free(a);
        big_int_free(b);
        big_int_free(ab);
        big_int_free(c);
    }

#undef LIST
}

void test_big_int_positive_sub ()
{
#define LIST {     \
    X(0, 0),       \
    X(0, 1),       \
    X(1, 0),       \
    X(100, 200),   \
    X(13, 17),     \
    X(45, 23),     \
    X(999, 99999), \
    X(99, 999),    \
    X(9, 11),      \
}

#define X(a, b) {MAX(a, b), MIN(a, b)}
    int input[][2] = LIST;
#undef X

#define X(a, b) ABS(MAX(a, b)) - ABS(MIN(a ,b))
    int check_sub[] = LIST;
#undef X

    for (int i = 0; i < sizeof(check_sub) / sizeof(*check_sub); ++i)
    {
        BigInt *a  = big_int_new(input[i][0], 0);
        BigInt *b  = big_int_new(input[i][1], 0);
        BigInt *ab = big_int_positive_sub(a, b);
        BigInt *c  = big_int_new(check_sub[i], 0);
        CHECK_COND(big_int_cmp(ab, c) == 0);
        big_int_free(a);
        big_int_free(b);
        big_int_free(ab);
        big_int_free(c);
    }

#undef LIST
}

void test ()
{
#define RUN_TEST(t)                       \
    do                                    \
    {                                     \
        t();                              \
        if (is_fail)                      \
        {                                 \
            printf("Ошибка в: %s\n", #t); \
            return;                       \
        }                                 \
    } while (0)

    RUN_TEST(test_big_new);
    RUN_TEST(test_big_int_to_cstr);
    RUN_TEST(test_big_int_cmp);
    RUN_TEST(test_big_int_positive_sum);
    RUN_TEST(test_big_int_positive_sub);

#undef RUN_TEST

    printf("Все ОК.");
}
