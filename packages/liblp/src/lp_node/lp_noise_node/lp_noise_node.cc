#include "../../duktape/duktape.h"
#include "../../util/noise.h"
#include "../../util/lp_duktape.h"
#include "./lp_noise_node.h"
#include "../lp_node.h"

const lp_node_type_def lp_noise_node_def = {
  .node_type_id = "noise",
  .instance_template_json = R"(
{
  "objType": "node_type_def",
  "nodeType": "noise",
  "input": {
    "pos": [ 0, 0, 0 ],
    "octaves": 1,
    "scale": 1
  },
  "output": 0,
  "config": {},
  "state": {}
}
)",
  .init_func = lp_noise_node_init,
  .eval_func = lp_noise_node_eval,
  .destroy_func = lp_noise_node_destroy,
};

int lp_noise_node_init(
  lp_context *lp_ctx
) {
  return 0;
}

int lp_noise_node_eval(
  lp_context *lp_ctx,
  int scopeIdx,
  int nodeIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  duk_get_prop_string(duk_ctx, nodeIdx, "input");

  lp_vec3 pos = lpduk_named_vec3(duk_ctx, "pos");
  auto octaves = lpduk_named_int(duk_ctx, "octaves");
  auto scale = lpduk_named_double(duk_ctx, "scale");

  duk_pop(duk_ctx); // input

  double result = perlin3(
    pos.x,
    pos.y,
    pos.z,

    octaves,
    scale
  );

  lpduk_set_named_double(duk_ctx, "output", result);

  return 0;
}

int lp_noise_node_destroy(
  lp_context *lp_ctx
) {
  return 0;
}
