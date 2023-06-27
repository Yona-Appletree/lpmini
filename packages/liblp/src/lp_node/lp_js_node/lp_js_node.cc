#include "../../duktape/duktape.h"
#include "../../util/noise.h"
#include "../../util/lp_duktape.h"
#include "./lp_js_node.h"

const lp_node_type_def lp_js_node_def = {
  .node_type_id = "js",
  .instance_template_json = R"(
{
  "objType": "node_instance",
  "nodeType": "js",
  "input": {},
  "output": {},
  "config": {
    "initJs": null,
    "updateJs": null,
    "evalJs": null,
    "destroyJs": null
  },
  "state": {},
  "connections": {}
}
)",
  .init_func = lp_js_node_init,
  .update_func = lp_js_node_update,
  .eval_func = lp_js_node_eval,
  .destroy_func = lp_js_node_destroy
};


int lp_js_node_init(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  LP_START_FUNC

  duk_get_prop_string(duk_ctx, nodeIdx, "config");
  int configIdx = duk_normalize_index(duk_ctx, -1);

  duk_get_prop_string(duk_ctx, nodeIdx, "state");
  int stateIdx = duk_normalize_index(duk_ctx, -1);

  // Init function
  duk_get_prop_string(duk_ctx, configIdx, "initJs");
  if (!duk_is_null_or_undefined(duk_ctx, -1)) {
    duk_push_string(duk_ctx, "lp_js_node_initJs"); // filename
    duk_compile(duk_ctx, DUK_COMPILE_FUNCTION | DUK_COMPILE_STRICT);
    duk_put_prop_string(duk_ctx, stateIdx, "initFn");
  } else {
    duk_pop(duk_ctx);
  }

  // Update function
  duk_get_prop_string(duk_ctx, configIdx, "updateJs");
  if (!duk_is_null_or_undefined(duk_ctx, -1)) {
    duk_push_string(duk_ctx, "lp_js_node_updateJs"); // filename
    duk_compile(duk_ctx, DUK_COMPILE_FUNCTION | DUK_COMPILE_STRICT);
    duk_put_prop_string(duk_ctx, stateIdx, "updateFn");
  } else {
    duk_pop(duk_ctx);
  }

  // Eval function
  duk_get_prop_string(duk_ctx, configIdx, "evalJs");
  if (!duk_is_null_or_undefined(duk_ctx, -1)) {
    duk_push_string(duk_ctx, "lp_js_node_evalJs"); // filename
    duk_compile(duk_ctx, DUK_COMPILE_FUNCTION | DUK_COMPILE_STRICT);
    duk_put_prop_string(duk_ctx, stateIdx, "evalFn");
  } else {
    duk_pop(duk_ctx);
  }

  // Destroy function
  duk_get_prop_string(duk_ctx, configIdx, "destroyJs");
  if (!duk_is_null_or_undefined(duk_ctx, -1)) {
    duk_push_string(duk_ctx, "lp_js_node_destroyJs"); // filename
    duk_compile(duk_ctx, DUK_COMPILE_FUNCTION | DUK_COMPILE_STRICT);
    duk_put_prop_string(duk_ctx, stateIdx, "destroyFn");
  } else {
    duk_pop(duk_ctx);
  }

  // Call the init function
  duk_get_prop_string(duk_ctx, stateIdx, "initFn");
  if (duk_is_function(duk_ctx, -1)) {
    duk_call(duk_ctx, 0);
    duk_put_prop_string(duk_ctx, nodeIdx, "state");
  } else {
    duk_pop(duk_ctx);
  }

  duk_pop_2(duk_ctx); // config, state

  LP_END_FUNC(0)
}


int lp_js_node_update(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  LP_START_FUNC

  duk_get_prop_string(duk_ctx, nodeIdx, "state");
  int stateIdx = duk_normalize_index(duk_ctx, -1);
  // [..., state]

  // Call the update function
  duk_get_prop_string(duk_ctx, stateIdx, "updateFn");
  if (duk_is_function(duk_ctx, -1)) {
    duk_dup(duk_ctx, stateIdx);
    duk_get_prop_string(duk_ctx, nodeIdx, "input");
    duk_call(duk_ctx, 1);
  }

  // [..., state, null | updateFnResult]
  duk_pop_2(duk_ctx);

  LP_END_FUNC(0)
}


int lp_js_node_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  LP_START_FUNC

  duk_get_prop_string(duk_ctx, nodeIdx, "state");
  int stateIdx = duk_normalize_index(duk_ctx, -1);
  // [..., state]

  // Call the eval function
  duk_get_prop_string(duk_ctx, stateIdx, "evalFn");
  if (duk_is_function(duk_ctx, -1)) {
    duk_dup(duk_ctx, stateIdx);
    duk_get_prop_string(duk_ctx, nodeIdx, "input");
    duk_call(duk_ctx, 2);
    duk_put_prop_string(duk_ctx, nodeIdx, "output");
  } else {
    // [..., state, null]
    duk_pop(duk_ctx);
  }

  // [..., state]

  duk_pop(duk_ctx);

  LP_END_FUNC(0)
}


int lp_js_node_destroy(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  LP_START_FUNC

  duk_get_prop_string(duk_ctx, nodeIdx, "state");
  int stateIdx = duk_normalize_index(duk_ctx, -1);
  // [..., state]

  // Call the destroy function
  duk_get_prop_string(duk_ctx, stateIdx, "destroyFn");
  if (duk_is_function(duk_ctx, -1)) {
    duk_dup(duk_ctx, stateIdx);
    duk_call(duk_ctx, 1);
  }

  // [..., state, null | destroyFnResult]

  duk_pop_2(duk_ctx);

  LP_END_FUNC(0)
}
