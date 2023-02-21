#include "duktape/duktape.h"
#include "nodes/lp_node_noise.h"
#include "util/lp_duktape.h"

extern "C" {
duk_context *lpapi_context_create(
  const char *input_json,
  const char *output_json
) {
  auto ctx = duk_create_heap_default();

  duk_push_bare_object(ctx);
  duk_put_global_string(ctx, "nodes");

  duk_push_bare_object(ctx);
  duk_put_global_string(ctx, "connections");

  duk_push_string(ctx, input_json);
  duk_json_decode(ctx, -1);
  duk_put_global_string(ctx, "input");

  duk_push_string(ctx, output_json);
  duk_json_decode(ctx, -1);
  duk_put_global_string(ctx, "output");

  return ctx;
}

void lpapi_context_destroy(
  duk_context *ctx
) {
  duk_destroy_heap(ctx);
}

void lpapi_add_node(
  duk_context *ctx,
  const char *type,
  const char *id
) {
  if (strcmp(type, "noise") == 0) {
    lpduk_create_node(ctx, id, lp_node_noise_init);
  }
}

void lpapi_eval_node(
  duk_context *ctx,
  const char *id
) {
  // []
  duk_get_global_string(ctx, "nodes");
  // [nodes]
  duk_get_prop_string(ctx, -1, id);
  // [nodes, node]
  duk_get_prop_string(ctx, -1, "evaluate");
  // [nodes, node, node.evaluate]
  duk_dup(ctx, -2);
  // [nodes, node, node.evaluate, node]
  duk_call_method(ctx, 0);
  // [nodes, node, node]
  duk_pop_3(ctx);
  // []
}

const char *lpapi_eval_expr(
  duk_context *ctx,
  const char *expr
) {
  duk_eval_string(ctx, expr);
  duk_json_encode(ctx, -1);
  auto json = duk_get_string(ctx, -1);
  duk_pop(ctx);
  return json;
}

}

