#include "../../duktape/duktape.h"
#include "../lp_node.h"

#ifndef lp_time_node_H
#define lp_time_node_H

extern const lp_node_type_def lp_time_node_def;

int lp_time_node_update(
  lp_context *ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

#endif
