#include "big_int.h"

int main ()
{
    /*test();*/

    BigInt *a = big_int_new(1, 1000);
    for (int i = 1; i <= 100; ++i)
    {
        BigInt *b = big_int_new(i, 10);
        BigInt *c = big_int_mul(a, b);
        big_int_free(a);
        a = c;
    }

    println_big_int(a);

    big_int_free(a);

    return 0;
}
