#include "../lp_context/lp_context.h"
#include "../lp_obj/lp_obj.h"
#include "../util/lp_duktape.h"
#include "lp_conn.h"
#include "../lp_scope/lp_scope.h"
#include "../lp_node/lp_node.h"

int lp_conn_create(
  lp_context *lp_ctx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  LP_OBJ_ASSERT_STACK(lp_obj_connection_def)

  // All we need to do is change the type
  duk_push_string(duk_ctx, lp_obj_connection_instance);
  duk_put_prop_string(duk_ctx, -2, "objType");

  LP_OBJ_ASSERT_STACK(lp_obj_connection_instance)
  return 0;
}

int lp_conn_path_to_string(
  duk_context *duk_ctx,
  int pathIdx
) {
  if (!duk_is_array(duk_ctx, pathIdx)) {
    duk_error(duk_ctx, DUK_ERR_ERROR, "Path must be an array");
  }

  duk_get_prop_string(duk_ctx, pathIdx, "join");
  duk_dup(duk_ctx, pathIdx);
  duk_push_string(duk_ctx, ".");
  duk_call_method(duk_ctx, 1);

  return duk_normalize_index(duk_ctx, -1);
}

int lp_conn_apply(
  lp_context *lp_ctx,

  int scopeIdx,
  int nodeIdx,
  int connectionIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  //printf("\n\n=============================\n");
  //printf("scopeIdx: %d, nodeIdx: %d, connectionIdx: %d\n", scopeIdx, nodeIdx, connectionIdx);
  duk_get_prop_string(duk_ctx, connectionIdx, lp_conn_js_sourceNodeId);
  bool source_is_scope;
  const char *sourcePropName;

  if (duk_is_null_or_undefined(duk_ctx, -1)) {
    source_is_scope = true;
    sourcePropName = lp_scope_js_input;

    duk_pop(duk_ctx); // pop the null
    duk_dup(duk_ctx, scopeIdx); // push the scope
  } else {
    source_is_scope = false;
    sourcePropName = lp_node_js_output;

    duk_get_prop_string(duk_ctx, scopeIdx, "nodes");
    duk_swap_top(duk_ctx, -2);
    duk_get_prop(duk_ctx, -2);

    if (duk_is_null_or_undefined(duk_ctx, -1)) {
      duk_get_prop_string(duk_ctx, connectionIdx, lp_conn_js_sourceNodeId);
      duk_error(duk_ctx, DUK_ERR_ERROR, "Node not found: %s", duk_to_string(duk_ctx, -1));
    }
  }
  // scope or node should be on top of the stack
  int sourceObjectIdx = duk_normalize_index(duk_ctx, -1);

  // Get the input
  duk_get_prop_string(duk_ctx, sourceObjectIdx, sourcePropName);
  int sourceRootIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, connectionIdx, lp_conn_js_inputPath);
  int inputPathIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, connectionIdx, lp_conn_js_outputPath);
  int outputPathIdx = duk_normalize_index(duk_ctx, -1);

  lpduk_get_path(duk_ctx, sourceRootIdx, inputPathIdx);
  int valueIdx = duk_normalize_index(duk_ctx, -1);

  // If node is null, we're setting the scope output, otherwise we're setting the node input
  duk_push_string(duk_ctx, duk_is_null_or_undefined(duk_ctx, nodeIdx) ? "output" : "input");
  int firstOutputKeyIdx = duk_normalize_index(duk_ctx, -1);

  int targetIdx = duk_is_null_or_undefined(duk_ctx, nodeIdx) ? scopeIdx : nodeIdx;

  // Debugging output
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
  if (false) {
    duk_get_prop_string(duk_ctx, connectionIdx, lp_conn_js_sourceNodeId);
    int sourceNodeIdIdx = duk_normalize_index(duk_ctx, -1);

    int sourceRootJsonIdx = lpduk_to_json(duk_ctx, sourceRootIdx);
    int inputPathJsonIdx = lp_conn_path_to_string(duk_ctx, inputPathIdx);
    int outputPathJsonIdx = lp_conn_path_to_string(duk_ctx, outputPathIdx);
    int valueJsonIdx = lpduk_to_json(duk_ctx, valueIdx);

    printf(
      "%s:%d (%s).%s.%s => %s.%s: %s\n",
      __FILE_NAME__,
      __LINE__,
      source_is_scope ? "<scope>" : duk_get_string(duk_ctx, sourceNodeIdIdx),
      duk_get_string(duk_ctx, sourceRootJsonIdx),
      duk_get_string(duk_ctx, inputPathJsonIdx),

      duk_get_string(duk_ctx, firstOutputKeyIdx),
      duk_get_string(duk_ctx, outputPathJsonIdx),

      duk_get_string(duk_ctx, valueJsonIdx)
    );

    duk_pop_n(duk_ctx, 5);
  }
#pragma clang diagnostic pop

  lpduk_set_path(duk_ctx, targetIdx, firstOutputKeyIdx, outputPathIdx, valueIdx);

  duk_pop_n(duk_ctx, 9);

  return 0;
}
