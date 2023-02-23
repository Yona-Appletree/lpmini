#include "../../duktape/duktape.h"
#include "../../util/noise.h"
#include "../../util/lp_duktape.h"
#include "./lp_expr_node.h"

const lp_node_type_def lp_expr_node_def = {
  .node_type_id = "expr",
  .instance_template_json = R"(
{
  "objType": "node_instance",
  "nodeType": "expr",
  "input": {
    "x": 0,
    "y": 0,
    "z": 0
  },
  "output": {
    "value": 0,
    "exprFn": null
  },
  "config": {
    "expr": "x + y + z"
  },
  "state": {},
  "connections": {}
}
)",
  .init_func = lp_expr_node_init,
  .update_func = nullptr,
  .eval_func = lp_expr_node_eval,
  .destroy_func = nullptr,
};


int lp_expr_node_init(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;
  duk_get_prop_string(duk_ctx, nodeIdx, "config");
  int configIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, nodeIdx, "state");
  int stateIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, nodeIdx, "output");
  int outputIdx = duk_normalize_index(duk_ctx, -1);

  // Compile as a function, see https://duktape.org/api.html#duk_compile
  duk_push_string(duk_ctx, "function(x,y,z){return ");
  duk_get_prop_string(duk_ctx, configIdx, "expr");
  duk_push_string(duk_ctx, "}");
  duk_concat(duk_ctx, 3);

  duk_push_string(duk_ctx, "lp_expr_node"); // filename

  duk_compile(duk_ctx, DUK_COMPILE_FUNCTION | DUK_COMPILE_STRICT);
  duk_dup(duk_ctx, -1);

  // Store the compiled function in the node instance
  duk_put_prop_string(duk_ctx, stateIdx, "compiledExpr");
  duk_put_prop_string(duk_ctx, outputIdx, "exprFn");

  duk_pop_3(duk_ctx); // config, state
  return 0;
}

int lp_expr_node_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;
  duk_get_prop_string(duk_ctx, nodeIdx, "input");
  int inputIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, nodeIdx, "output");
  int outputIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, nodeIdx, "state");
  int stateIdx = duk_normalize_index(duk_ctx, -1);

  // Call the compiled expression
  duk_get_prop_string(duk_ctx, stateIdx, "compiledExpr");
  duk_get_prop_string(duk_ctx, inputIdx, "x");
  duk_get_prop_string(duk_ctx, inputIdx, "y");
  duk_get_prop_string(duk_ctx, inputIdx, "z");
  duk_call(duk_ctx, 3);

  // Store the result in the output
  duk_put_prop_string(duk_ctx, outputIdx, "value");

  duk_pop_3(duk_ctx); // input, output, state

  return 0;
}
