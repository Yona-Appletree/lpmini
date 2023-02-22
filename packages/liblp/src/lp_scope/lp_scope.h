#ifndef LP_SCOPE_H
#define LP_SCOPE_H

#include "../lp_context/lp_context.h"

/**
 * Creates a new scope, expects the lp_scope_def to be on the top of the stack.
 * Leaves the new scope on the top of the stack.
 *
 * @param lp_ctx
 * @return
 */
int contextIdx(
  lp_context *lp_ctx,
  int contextIdx
);

/**
 * Evaluates all nodes in a scope, expects the scope to be on the top of the stack.
 *
 * @param lp_ctx
 * @return
 */
int lp_scope_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx
);

#endif
