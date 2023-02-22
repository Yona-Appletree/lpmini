#ifndef LP_NODE_H
#define LP_NODE_H

#include "../duktape/duktape.h"
#include "../lp_context/lp_context.h"

typedef struct {
    const char *node_type_id;
    const char *instance_template_json;

    int (*init_func)(lp_context *);

    int (*eval_func)(
      lp_context *,
      int,
      int
    );

    int (*destroy_func)(lp_context *);
} lp_node_type_def;

const lp_node_type_def *lp_node_type_def_by_id(const char *node_type_id);

/**
 * Create a node from it's definition.
 *
 * Expects the the node scope to be on the top of the stack.
 * Leaves the new node on the top of the stack.
 *
 * @param ctx
 * @param id
 * @param node_type_def
 * @return
 */
int lp_node_create(
  lp_context *lp_ctx,
  const char *nodeId
);

/**
 * Evaluate a given node, assuming it's on the top of the stack.
 *
 * @param lp_ctx
 * @return
 */
int lp_node_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

#endif
