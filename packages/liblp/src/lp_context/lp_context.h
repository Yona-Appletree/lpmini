#ifndef LP_CONTEXT_H
#define LP_CONTEXT_H

#include "../duktape/duktape.h"

/**
 * Name of the global variable that contains the context.
 */
static auto lp_context_global_js = "context";
static auto lp_context_def_js_rootScopeDef = "rootScopeDef";
static auto lp_context_instance_js_rootScope = "rootScope";
static auto lp_context_instance_js_log = "log";

typedef struct {
    duk_context *duk_ctx;
    int frame_counter;
    duk_size_t heap_size;
} lp_context;

lp_context *lp_context_create(
  const char *lp_context_def_json
);

void lp_context_update(
  lp_context *lp_ctx
);

void lp_context_destroy(
  lp_context *lp_ctx
);

void lp_context_eval(
  lp_context *lp_ctx
);

/**
 * Converts the context to JSON, leaves it on the stack.
 *
 * @param lp_ctx
 */
void lp_context_to_json(
  lp_context *lp_ctx
);

/**
 * Evaluates a javascript expression in the context of the root scope, leaves the result on the stack.
 *
 * @param lp_ctx
 * @param js
 */
void lp_context_eval_js(
  lp_context *lp_ctx,
  const char *js
);

double lp_context_eval_js_number(
  lp_context *lp_ctx,
  const char *js
);


#endif
