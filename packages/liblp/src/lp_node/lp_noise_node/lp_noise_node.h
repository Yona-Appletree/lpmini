#include "../../duktape/duktape.h"
#include "../lp_node.h"

#ifndef LP_noise_node_H
#define LP_noise_node_H

extern const lp_node_type_def lp_noise_node_def;

int lp_noise_node_init(
  lp_context *ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

int lp_noise_node_eval(
  lp_context *ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

int lp_noise_node_update(
  lp_context *ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

int lp_noise_node_destroy(
  lp_context *ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

#endif
