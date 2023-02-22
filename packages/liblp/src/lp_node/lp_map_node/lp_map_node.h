#include "../../duktape/duktape.h"
#include "../lp_node.h"

#ifndef lp_map_node_H
#define lp_map_node_H

extern const lp_node_type_def lp_map_node_def;

int lp_map_node_eval(
  lp_context *ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

#endif
