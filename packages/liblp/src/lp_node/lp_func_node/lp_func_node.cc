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
  "input": {},
  "output": {
    "func": null
  },
  "config": {
    "scope": null
  },
  "state": {},
  "connections": {}
}
)",
  .init_func = lp_func_node_init,
  .update_func = nullptr,
  .eval_func = lp_func_node_eval,
  .destroy_func = lp_func_node_destroy,
};

/**
 * The function that should actually evaluate the node.
 *
 * @param duk_ctx
 * @return
 */
duk_ret_t lp_func_node_jsFunc(duk_context *duk_ctx) {
  // [args]
  int argsIdx = duk_normalize_index(duk_ctx, -1);

  duk_push_this(duk_ctx);
  int funcContextIdx = duk_normalize_index(duk_ctx, -1);
  // [args, this]

  duk_get_prop_index(duk_ctx, funcContextIdx, 0);
  lp_context *lp_ctx = (lp_context *) duk_get_pointer(duk_ctx, -1);
  duk_pop(duk_ctx);

  duk_get_prop_index(duk_ctx, funcContextIdx, 1);
  int contextIdx = duk_normalize_index(duk_ctx, -1);
  duk_get_prop_index(duk_ctx, funcContextIdx, 2);
  int scopeIdx = duk_normalize_index(duk_ctx, -1);
  duk_get_prop_index(duk_ctx, funcContextIdx, 3);
  int nodeIdx = duk_normalize_index(duk_ctx, -1);

  // TODO: Optimize this, we shouldn't need a temp variable
  duk_push_bare_object(duk_ctx);
  int ioIdx = duk_normalize_index(duk_ctx, -1);
  // [args, this, state, scope, io]

  duk_dup(duk_ctx, argsIdx);
  duk_put_prop_string(duk_ctx, ioIdx, "input");
  // [args, this, state, scope, io]

  lp_scope_eval(lp_ctx, contextIdx, scopeIdx, ioIdx);

  duk_get_prop_string(duk_ctx, ioIdx, "output");
  // [args, this, state, scope, io, output]
  duk_swap(duk_ctx, argsIdx, -1);
  // [output, this, state, scope, io, args]
  duk_pop_n(duk_ctx, 5);

  return 0;
}

int lp_func_node_init(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  LP_START_FUNC

  // [...]

  duk_get_prop_string(duk_ctx, nodeIdx, "config");
  int configIdx = duk_normalize_index(duk_ctx, -1);
  // [..., config]

  duk_get_prop_string(duk_ctx, nodeIdx, "state");
  int stateIdx = duk_normalize_index(duk_ctx, -1);
  // [..., config, state]

  duk_get_prop_string(duk_ctx, nodeIdx, "output");
  int outputIdx = duk_normalize_index(duk_ctx, -1);
  // [..., config, state, output]

  // Build the scope
  duk_get_prop_string(duk_ctx, configIdx, "scope");
  // [..., config, state, output, scope_def]
  lp_scope_create(lp_ctx, -1);
  // [..., config, state, output, scope_instance]

  // Store the scope in our state
  duk_put_prop_string(duk_ctx, stateIdx, "scope");
  // [..., config, state, output]

  // Create the function
  duk_push_c_function(duk_ctx, lp_func_node_jsFunc, 1);
  // [..., config, state, output, func]

  // Bind the function to the node instance
  duk_get_prop_string(duk_ctx, configIdx, "bind");
  // [..., config, state, output, func, bind]
  duk_swap_top(duk_ctx, -2);
  // [..., config, state, output, bind, func]
  duk_push_bare_array(duk_ctx);
  int funcContextIdx = LP_STACK_TOP;
  // [..., config, state, output, bind, func, {}]
  duk_push_pointer(duk_ctx, lp_ctx);
  duk_put_prop_index(duk_ctx, funcContextIdx, 0);
  duk_dup(duk_ctx, contextIdx);
  duk_put_prop_index(duk_ctx, funcContextIdx, 1);
  duk_dup(duk_ctx, scopeIdx);
  duk_put_prop_index(duk_ctx, funcContextIdx, 2);
  duk_dup(duk_ctx, nodeIdx);
  duk_put_prop_index(duk_ctx, funcContextIdx, 3);

  duk_call_method(duk_ctx, 1);
  // [..., config, state, output, bound_func]
  duk_put_prop_string(duk_ctx, outputIdx, "func");
  // [..., config, state, output]

  duk_pop_3(duk_ctx);
  // [...]

  LP_END_FUNC(0)
}

int lp_func_node_destroy(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  LP_START_FUNC

  // [...]
  duk_get_prop_string(duk_ctx, nodeIdx, "state");
  // [..., state]
  duk_get_prop_string(duk_ctx, -1, "scope");
  // [..., state, scope]
  lp_scope_destroy(lp_ctx, contextIdx, LP_STACK_TOP);
  // [..., state]
  duk_pop(duk_ctx);
  // [...]

  LP_END_FUNC(0)
}

int lp_func_node_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  LP_START_FUNC

  LP_END_FUNC(0)
}
