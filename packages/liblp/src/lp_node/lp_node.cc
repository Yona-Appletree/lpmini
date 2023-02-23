#include "lp_node.h"
#include "lp_noise_node/lp_noise_node.h"
#include "../util/lp_duktape.h"
#include "../lp_obj/lp_obj.h"
#include "lp_expr_node/lp_expr_node.h"
#include "lp_map_node/lp_map_node.h"
#include "lp_func_node/lp_func_node.h"
#include "lp_time_node/lp_time_node.h"

const lp_node_type_def *lp_node_type_defs[] = {
  &lp_noise_node_def,
  &lp_expr_node_def,
  &lp_map_node_def,
  &lp_func_node_def,
  &lp_time_node_def,
  nullptr
};

const lp_node_type_def *lp_node_type_def_by_id(const char *node_type_id) {
  for (int i = 0; lp_node_type_defs[i] != nullptr; i++) {
    if (strcmp(lp_node_type_defs[i]->node_type_id, node_type_id) == 0) {
      return lp_node_type_defs[i];
    }
  }

  return nullptr;
}

int lp_node_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  duk_get_prop_string(duk_ctx, nodeIdx, "nodeType");
  const char *node_type_id = duk_get_string(duk_ctx, -1);
  const lp_node_type_def *node_type_def = lp_node_type_def_by_id(node_type_id);
  duk_pop(duk_ctx);

  if (node_type_def->eval_func != nullptr) {
    node_type_def->eval_func(lp_ctx, contextIdx, scopeIdx, nodeIdx);
  }

  return 0;
}

int lp_node_update(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  duk_get_prop_string(duk_ctx, nodeIdx, "nodeType");
  const char *node_type_id = duk_get_string(duk_ctx, -1);
  const lp_node_type_def *node_type_def = lp_node_type_def_by_id(node_type_id);
  duk_pop(duk_ctx);

  if (node_type_def->update_func != nullptr) {
    node_type_def->update_func(lp_ctx, contextIdx, scopeIdx, nodeIdx);
  }

  return 0;
}

int lp_node_create(
  lp_context *lp_ctx,
  const char *nodeId,
  int contextId,
  int scopeIdx,
  int nodeDefIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  LP_OBJ_ASSERT_STACK_INDEX(lp_obj_node_def, nodeDefIdx)

  // PRINT_DUK_STACK // [node_def]

  duk_get_prop_string(duk_ctx, nodeDefIdx, "nodeType");
  const char *node_type_id = duk_get_string(duk_ctx, -1);
  duk_pop(duk_ctx);

  const lp_node_type_def *node_type_def = lp_node_type_def_by_id(node_type_id);

  // Create the node instance
  duk_push_string(duk_ctx, node_type_def->instance_template_json);
  duk_json_decode(duk_ctx, -1);
  int nodeInstIdx = duk_normalize_index(duk_ctx, -1);

  // Ensure connections are present
  lpduk_set_prop_empty_object(duk_ctx, nodeInstIdx, "connections");

  // Apply the node definition to the node instance
  lpduk_apply_partial(duk_ctx, 1, nodeInstIdx, nodeDefIdx);

  // Update connection types
  duk_get_prop_string(duk_ctx, nodeInstIdx, "connections");
  duk_enum(duk_ctx, -1, DUK_ENUM_OWN_PROPERTIES_ONLY);
  while (duk_next(duk_ctx, -1, 1)) {
    lpduk_set_prop_literal(duk_ctx, -1, "objType", lp_obj_connection_instance);
    duk_pop_2(duk_ctx);
  }
  duk_pop_2(duk_ctx);

  // Assign values
  lpduk_set_prop_literal(duk_ctx, nodeInstIdx, "objType", lp_obj_node_instance);
  lpduk_set_prop_literal(duk_ctx, nodeInstIdx, "nodeId", nodeId);

  // [node_instance]

  LP_OBJ_ASSERT_STACK(lp_obj_node_instance)

  if (node_type_def->init_func != nullptr) {
    node_type_def->init_func(lp_ctx, contextId, scopeIdx, nodeInstIdx);
  }

  return 0;
}
