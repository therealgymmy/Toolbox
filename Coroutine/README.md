Coroutine
=========

####Status

* a basic C89 implementation is now in place with the following caveats:
* linux implementation is achieved through `ucontext.h`, which isn't the best idea...
* windows implementation is mature enough that I use it daily in my own projects... but no guarantees!

Introduction
------------

A header-only library for coroutines!

The plan is to have both a barebone C89 library and
a C++11 wrapper on top for advanced features.

To try it out now, simply copy the source code in `./clib/src/` and
put `#include "coroutine.h"` in the desired file.

A Simple Example
----------------

###C89

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
    
        param.stack_size = 0;
        param.data       = &stuff;
    
        cordata_ptr cor_foo = cor_new(foo1, &param);
        
        for (i = 0; i < 10; ++i) {
            printf("main: %d and %d\n", i, stuff);
            cor_call(cor_foo, NULL);
        }
    
        cor_del(cor_foo);
        cor_del(g_cor);
    
        return 0;
    }
    
###C++11

todo
