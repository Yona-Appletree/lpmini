#include "../duktape/duktape.h"
#include "../util/noise.h"
#include "../util/lp_duktape.h"
#include "./lp_node_noise.h"

const char *initJson = R"V0G0N(
{
  "type": "noise",
  "input": {
    "pos": [0,0,0],
    "octaves": 1,
    "scale": 0.5
  },
  "output": 0
}
)V0G0N";

int lp_node_noise_init(
  duk_context *ctx
) {
  return lpduk_push_node(ctx, initJson, lp_node_noise_eval);
}

int lp_node_noise_eval(
  duk_context *ctx
) {
  duk_push_this(ctx);

  duk_get_prop_string(ctx, -1, "input");

  lp_vec3 pos = lpduk_named_vec3(ctx, "pos");
  auto octaves = lpduk_named_int(ctx, "octaves");
  auto scale = lpduk_named_double(ctx, "scale");

  duk_pop(ctx); // input

  double result = perlin3(
    pos.x,
    pos.y,
    pos.z,

    octaves,
    scale
  );

  lpduk_set_named_double(ctx, "output", result);

  duk_pop(ctx); // this
  return 0;
}
