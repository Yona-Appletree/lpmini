#ifndef LP_CONN_H
#define LP_CONN_H

#include "../lp_context/lp_context.h"

static const char *lp_conn_js_sourceNodeId = "sourceNodeId";
static const char *lp_conn_js_inputPath = "inputPath";
static const char *lp_conn_js_outputPath = "outputPath";

int lp_conn_create(
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
int lp_conn_apply(
  lp_context *lp_ctx,

  int scopeIdx,
  int nodeIdx,
  int connectionIdx
);

#endif
