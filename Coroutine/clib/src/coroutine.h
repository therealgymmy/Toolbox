#ifndef COROUTINE_H
#define COROUTINE_H

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* void pointer type */
typedef void *void_ptr;

/* entry point for a coroutine */
typedef void (*entry_fn)(void_ptr);

/*
 * parameters for creating a new coroutine
 *
 * fields:
 * stack_size -> [IN] specify the size of coroutine stack
 *                    0 means to use the default stack size
 * data       -> [IN] void_ptr to user defined data
 *
 */
typedef struct corparam {
    size_t   stack_size;
    void_ptr data;
} corparam, *corparam_ptr;

/*
 * data structure for coroutine
 *
 * fields:
 * handler -> [OUT] handler to coroutine
 */
typedef struct cordata {
    void_ptr handler;
} cordata, *cordata_ptr;

/*
 * initialize coroutine library on the current thread
 *
 * precondition:
 * non-reentrent on the same thread
 *
 * parameter:
 * param -> [IN, OPT] parameters for coroutine creation
 *
 * on success:
 * return -> cordata_ptr
 *
 * on failure:
 * return -> NULL
 *        -> cordata_ptr::coroutine_handler == NULL
 */
cordata_ptr cor_init(void_ptr data);

/*
 * create new coroutine
 *
 * parameter:
 * fn    -> [IN] entry point
 * param -> [IN, OPT] parameters for coroutine creation
 *
 * on success:
 * return -> cordata_ptr
 *
 * on failure:
 * return -> NULL
 *        -> cordata_ptr::coroutine_handler == NULL
 */
cordata_ptr cor_new(entry_fn fn, corparam_ptr param);

/*
 * delete existing coroutine
 */
void cor_del(cordata_ptr target);

/*
 * invoke target coroutine
 *
 * parameter:
 * target -> [IN] coroutine to switch to
 * data   -> [IN, OPT] data to pass to coroutine
 *
 * on success:
 * return -> pointer to data returned by target
 *
 * on failure:
 * behavior undefined
 *
 */
void_ptr cor_call(cordata_ptr target, void_ptr data);

#if defined(__cplusplus)
}
#endif

#endif
