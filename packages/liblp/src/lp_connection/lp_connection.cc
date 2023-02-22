#include "../lp_context/lp_context.h"
#include "../lp_obj/lp_obj.h"
#include "../util/lp_duktape.h"

int lp_connection_create(
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

int lp_connection_apply(
  lp_context *lp_ctx,

  int scopeIdx,
  int nodeIdx,
  int connectionIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  duk_get_prop_string(duk_ctx, connectionIdx, "sourceNodeId");
  if (!duk_is_null_or_undefined(duk_ctx, -1)) {
    duk_get_prop_string(duk_ctx, scopeIdx, "nodes");
    duk_swap_top(duk_ctx, -2);

    duk_get_prop(duk_ctx, -2);
  }

  // node should be on top of the stack

  if (duk_is_null_or_undefined(duk_ctx, -1)) {
    // If null just use the scope
    duk_pop(duk_ctx);
    duk_dup(duk_ctx, scopeIdx);
  }

  // Get the input
  duk_get_prop_string(duk_ctx, -1, "input");
  int sourceNodeIdIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, connectionIdx, "inputPath");
  int inputPathIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, connectionIdx, "outputPath");
  int outputPathIdx = duk_normalize_index(duk_ctx, -1);

  lpduk_get_path(duk_ctx, sourceNodeIdIdx, inputPathIdx);
  int valueIdx = duk_normalize_index(duk_ctx, -1);

  lpduk_set_path(duk_ctx, nodeIdx, outputPathIdx, valueIdx);

  PRINT_DUK_STACK
  duk_pop_n(duk_ctx, 6);

  return 0;
}
