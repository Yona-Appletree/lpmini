#include "../../duktape/duktape.h"
#include "../lp_node.h"

#ifndef lp_func_node_H
#define lp_func_node_H

extern const lp_node_type_def lp_func_node_def;

int lp_func_node_init(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

int lp_func_node_update(
  lp_context *ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

int lp_func_node_eval(
  lp_context *ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

int lp_func_node_destroy(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

#endif
