#ifndef LP_NODE_H
#define LP_NODE_H

#include "../duktape/duktape.h"
#include "../lp_context/lp_context.h"

typedef int (*lp_node_lifecycle_func)(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

typedef struct {
    const char *node_type_id;
    const char *instance_template_json;

    lp_node_lifecycle_func init_func;
    lp_node_lifecycle_func update_func;
    lp_node_lifecycle_func eval_func;
    lp_node_lifecycle_func destroy_func;
} lp_node_type_def;

const lp_node_type_def *lp_node_type_def_by_id(const char *node_type_id);

/**
 * Create a node from it's definition.
 *
 * Pushes the new node on the top of the stack.
 *
 * @param ctx
 * @param id
 * @param node_type_def
 * @return
 */
int lp_node_create(
  lp_context *lp_ctx,
  const char *nodeId,
  int contextId,
  int scopeIdx,
  int nodeDefIdx
);

/**
 * Evaluate a given node.
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

/**
 * Update a given node.
 *
 * @param lp_ctx
 * @return
 */
int lp_node_update(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
);

#endif
