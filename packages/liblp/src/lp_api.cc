#include "duktape/duktape.h"
#include "util/lp_duktape.h"
#include "lp_context/lp_context.h"

extern "C" {
lp_context *lp_api_context_create(
  const char *lp_context_def_json
) {
  return lp_context_create(lp_context_def_json);
}

void lp_api_context_destroy(
  lp_context *lp_ctx
) {
  lp_context_destroy(lp_ctx);
}

void lp_api_add_node(
  lp_context *lp_ctx,
  const char *type,
  const char *id
) {
  //if (strcmp(type, "noise") == 0) {
  //  lpduk_create_node(ctx, id, lp_noise_node_init);
  //}
}

void lp_api_eval_node(
  lp_context *lp_ctx,
  const char *id
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  // []
  duk_get_global_string(duk_ctx, "nodes");
  // [nodes]
  duk_get_prop_string(duk_ctx, -1, id);
  // [nodes, node]
  duk_get_prop_string(duk_ctx, -1, "evaluate");
  // [nodes, node, node.evaluate]
  duk_dup(duk_ctx, -2);
  // [nodes, node, node.evaluate, node]
  duk_call_method(duk_ctx, 0);
  // [nodes, node, node]
  duk_pop_3(duk_ctx);
  // []
}

const char *lp_api_eval_expr(
  lp_context *lp_ctx,
  const char *expr
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  duk_eval_string(duk_ctx, expr);
  duk_json_encode(duk_ctx, -1);
  auto json = duk_get_string(duk_ctx, -1);
  duk_pop(duk_ctx);
  return json;
}

void lp_api_eval_context(
  lp_context *lp_ctx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  duk_get_global_string(duk_ctx, "connections");

  PRINT_DUK_STACK
  duk_enum(duk_ctx, -1, DUK_ENUM_OWN_PROPERTIES_ONLY);
  PRINT_DUK_STACK
  while (duk_next(duk_ctx, -1, 1)) {
    PRINT_DUK_STACK
    duk_pop(duk_ctx);
    PRINT_DUK_STACK
  }
  duk_pop_2(duk_ctx);
  PRINT_DUK_STACK
}

}

