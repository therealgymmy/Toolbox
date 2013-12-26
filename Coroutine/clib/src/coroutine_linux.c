#ifdef __linux__

/*
 * Linux implementation for coroutine
 */

/* need this define to enable stack_t in signal.h */
#define _BSD_SOURCE

#include "coroutine.h"
#include <stdlib.h>
#include <ucontext.h>

typedef void (*ucontext_entry_fn)(void);

/*
 * fields:
 * stack_ps   -> [OUT] pointer to stack
 *                     NULL means initial coroutine stack
 * stack_size -> [OUT] stack size
 *                     0 means size unspecified
 */
typedef struct ucontext_data {
    void_ptr stack_sp;
    unsigned stack_size;
} ucontext_data, *ucontext_data_ptr;

static ucontext_t *cur_context = NULL;

cordata_ptr cor_init(void_ptr data) {

    cordata_ptr       cor          = NULL;
    ucontext_data_ptr context_data = NULL;

    if (!(cor = malloc(sizeof(cordata)))) {
        goto on_failure;
    }

    if (!(cur_context = malloc(sizeof(ucontext_t)))) {
        goto on_failure;
    }

    if (!(context_data = malloc(sizeof(ucontext_data)))) {
        goto on_failure;
    }

    context_data->stack_sp   = NULL;
    context_data->stack_size = 0;

    cor->handler   = cur_context;
    cor->arch_data = context_data;

on_success:
    return cor;

on_failure:
    free(context_data);
    free(cur_context);
    free(cor);
    return NULL;
}

cordata_ptr cor_new(entry_fn fn, corparam_ptr param) {

    cordata_ptr cor        = NULL;
    ucontext_t *context    = NULL;
    size_t      stack_size = 0;
    void_ptr    data       = NULL;
    void_ptr    stack_sp   = NULL;
    ucontext_data_ptr context_data = NULL;

    if (!(cor = malloc(sizeof(cordata)))) {
        goto on_failure;
    }

    if (param) {
        stack_size = param->stack_size;
        data       = param->data;
    }

    if (!(context = malloc(sizeof(ucontext_t)))) {
        goto on_failure;
    }

    if (!(context_data = malloc(sizeof(ucontext_data)))) {
        goto on_failure;
    }

    if (!(stack_sp = malloc(SIGSTKSZ))) {
        goto on_failure;
    }

    context_data->stack_sp   = stack_sp;
    context_data->stack_size = SIGSTKSZ;

    cor->handler   = context;
    cor->arch_data = context_data;

    getcontext(context);

    context->uc_link          = cur_context;
    context->uc_stack.ss_sp   = context_data->stack_sp;
    context->uc_stack.ss_size = context_data->stack_size;

    makecontext(context, (ucontext_entry_fn)fn, 1, data);

on_success:
    return cor;

on_failure:
    free(stack_sp);
    free(context_data);
    free(context);
    free(cor);
    return NULL;
}

void cor_del(cordata_ptr target) {
    ucontext_t        *context       = target->handler;
    ucontext_data_ptr  context_data  = target->arch_data;

    free(context_data->stack_sp);
    free(context_data);
    free(context);
    free(target);
}

void_ptr cor_call(cordata_ptr target, void_ptr data) {

    ucontext_t *temp = cur_context;

    cur_context = target->handler;

    swapcontext(temp, cur_context);

    return NULL;
}

#endif
