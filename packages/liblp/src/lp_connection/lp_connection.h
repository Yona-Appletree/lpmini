#ifndef LP_CONNECTION_H
#define LP_CONNECTION_H

#include "../lp_context/lp_context.h"

int lp_connection_create(
  lp_context *lp_ctx
);

/**
 * Applies a connection.
 *
 * Doesn't change the stack.
 *
 * @param lp_ctx
 * @return
 */
int lp_connection_apply(
  lp_context *lp_ctx,

  int scopeIdx,
  int nodeIdx,
  int connectionIdx
);

#endif
