#include "../../duktape/duktape.h"
#include "../../util/noise.h"
#include "../../util/lp_duktape.h"
#include "./lp_map_node.h"

const lp_node_type_def lp_map_node_def = {
  .node_type_id = "noise",
  .instance_template_json = R"(
{
  "objType": "node_instance",
  "nodeType": "map",
  "input": {
    "array": [],
    "mapFn": null
  },
  "output": [],
  "config": {},
  "state": {},
  "connections": {}
}
)",
  .init_func = nullptr,
  .update_func = nullptr,
  .eval_func = lp_map_node_eval,
  .destroy_func = nullptr,
};

int lp_map_node_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;
  duk_get_prop_string(duk_ctx, nodeIdx, "input");
  int inputIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, inputIdx, "array");
  int inputArrayIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, inputIdx, "mapFn");
  int mapFnIdx = duk_normalize_index(duk_ctx, -1);

  // Create the result array
  duk_push_array(duk_ctx);
  int outputArrayIdx = duk_normalize_index(duk_ctx, -1);

  // Store the array in the output
  duk_dup(duk_ctx, -1);
  duk_put_prop_string(duk_ctx, nodeIdx, "output");

  // Iterate the array/object
  duk_enum(duk_ctx, inputArrayIdx, 0);
  while (duk_next(duk_ctx, -2, 1)) {
    duk_dup(duk_ctx, mapFnIdx);
    duk_swap_top(duk_ctx, -2);

    // Call the function
    duk_call(duk_ctx, 1);

    // Store the result in the output array
    duk_put_prop(duk_ctx, outputArrayIdx);

    duk_pop(duk_ctx); // key
  }
  duk_pop_n(duk_ctx, 5); // input, input.array, input.mapFn, outputArray, enum

  return 0;
}
