#include "../src/coroutine.h"

#include <stdio.h>

cordata_ptr g_cor;

void foo1(void_ptr data) {
    int i;
    int *num = data;

    for (i = 0; i < 10; ++i) {
        ++*num;
        printf("foo1: %d\n", i);

        cor_call(g_cor, NULL);
    }
}

int main() {

    int i;
    int stuff = 0;
    corparam param;

    g_cor = cor_init(&stuff);

    printf("initialized\n");

    param.stack_size = 0;
    param.data = &stuff;

    cordata_ptr cor_foo = cor_new(foo1, &param);

    for (i = 0; i < 10; ++i) {
        printf("main: %d and %d\n", i, stuff);

        cor_call(cor_foo, NULL);
    }

    printf("main: %d\n", i);

    cor_del(cor_foo);
    cor_del(g_cor);

    printf("main: %d\n", i);

    return 0;
}
