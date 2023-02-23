#include "../../duktape/duktape.h"
#include "../../util/noise.h"
#include "../../util/lp_duktape.h"
#include "../../lp_scope/lp_scope.h"
#include "./lp_func_node.h"

const lp_node_type_def lp_func_node_def = {
  .node_type_id = "func",
  .instance_template_json = R"(
{
  "objType": "node_instance",
  "nodeType": "func",
  "input": {
    "array": [],
    "funcFn": null,
  },
  "output": {
    "value": []
  },
  "config": {
    "scope": null as (null | lp_scope_def)
  },
  "state": {},
  "connections": {}
}
)",
  .init_func = lp_func_node_init,
  .update_func = nullptr,
  .eval_func = lp_func_node_eval,
  .destroy_func = nullptr,
};


int lp_func_node_init(
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

  // Build the scope
  duk_get_prop_string(duk_ctx, scopeIdx, "scope");
  lp_scope_create(lp_ctx, -1);

  // Store the scope in our state
  duk_put_prop_string(duk_ctx, stateIdx, "scope");

  duk_pop_2(duk_ctx); // config, state, scopeDef
  return 0;
}

int lp_func_node_eval(
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

  // Call the compiled funcession
  duk_get_prop_string(duk_ctx, stateIdx, "compiledFunc");
  duk_get_prop_string(duk_ctx, inputIdx, "x");
  duk_get_prop_string(duk_ctx, inputIdx, "y");
  duk_get_prop_string(duk_ctx, inputIdx, "z");
  duk_call(duk_ctx, 3);

  // Store the result in the output
  duk_put_prop_string(duk_ctx, outputIdx, "value");

  duk_pop_3(duk_ctx); // input, output, state

  return 0;
}
