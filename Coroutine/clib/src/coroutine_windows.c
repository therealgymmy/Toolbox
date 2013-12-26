#ifdef _WIN32

/*
 * Windows implementation for coroutine
 */

#include "coroutine.h"
#include <windows.h>

typedef VOID __stdcall (*fiber_entry_fn)(LPVOID);

cordata_ptr cor_init(void_ptr data) {

    cordata_ptr cor;

    if (!(cor = malloc(sizeof(cordata)))) {
        return NULL;
    }

    cor->handler   = ConvertThreadToFiber(data);
    cor->arch_data = NULL;

    return cor;
}

cordata_ptr cor_new(entry_fn fn, corparam_ptr param) {

    cordata_ptr cor;
    size_t      stack_size = 0;
    void_ptr    data       = NULL;

    if (!(cor = malloc(sizeof(cordata)))) {
        return NULL;
    }

    if (param) {
        stack_size = param->stack_size;
        data       = param->data;
    }

    cor->handler   = CreateFiber(stack_size, (fiber_entry_fn)fn, data);
    cor->arch_data = NULL;

    return cor;
}

void cor_del(cordata_ptr target) {
    DeleteFiber(target->handler);
    free(target);
}

void_ptr cor_call(cordata_ptr target, void_ptr data) {
    SwitchToFiber(target->handler);

    return NULL;
}

#endif
