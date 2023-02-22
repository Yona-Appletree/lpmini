#include "../lp_context/lp_context.h"
#include "../lp_obj/lp_obj.h"

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
  LP_OBJ_ASSERT_STACK_INDEX(lp_obj_scope_instance, scopeIdx)
  LP_OBJ_ASSERT_STACK_INDEX(lp_obj_node_instance, nodeIdx)
  LP_OBJ_ASSERT_STACK_INDEX(lp_obj_connection_instance, connectionIdx)

  return 0;
}
