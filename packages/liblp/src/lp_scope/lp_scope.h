#ifndef LP_SCOPE_H
#define LP_SCOPE_H

#include "../lp_context/lp_context.h"

static const char *lp_scope_js_nodes = "nodes";
static const char *lp_scope_js_connections = "connections";
static const char *lp_scope_js_input = "input";
static const char *lp_scope_js_output = "output";

static const char *lp_scope_js[] = {
  lp_scope_js_nodes,
  lp_scope_js_connections,
  lp_scope_js_input,
  lp_scope_js_output,
  nullptr
};

/**
 * Creates a new scope, expects the lp_scope_def to be on the top of the stack.
 * Leaves the new scope on the top of the stack.
 *
 * @param lp_ctx
 * @return
 */
int lp_scope_create(
  lp_context *lp_ctx,
  int contextIdx
);

/**
 * Evaluates all nodes in a scope.
 *
 * @param lp_ctx
 * @return
 */
int lp_scope_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int scopeIoIdx
);

/**
 * Updates all nodes in a scope.
 *
 * @param lp_ctx
 * @param contextIdx
 * @param scopeIdx
 * @return
 */
int lp_scope_update(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx
);

int lp_scope_eval_js(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  const char *js
);

/**
 * Destroys the given scope, cleaning up any resources it may have allocated.
 *
 * Stack: [...] -> [...]
 *
 * @param lp_ctx
 */
void lp_scope_destroy(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx
);

#endif
