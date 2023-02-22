#include "../../duktape/duktape.h"
#include "../../util/noise.h"
#include "../../util/lp_duktape.h"
#include "./lp_time_node.h"
#include "../lp_node.h"
#include<sys/time.h>

const lp_node_type_def lp_time_node_def = {
  .node_type_id = "noise",
  .instance_template_json = R"(
{
  "objType": "node_instance",
  "nodeType": "time",
  "input": {},
  "output": {
    "frameCounter": 0,
    "elapsedMs": 0,
    "lastFrameDurationMs": 0
  },
  "config": {},
  "state": {},
  "connections": {}
}
)",
  .init_func = nullptr,
  .update_func = lp_time_node_update,
  .eval_func = nullptr,
  .destroy_func = nullptr,
};

int lp_time_node_update(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx,
  int nodeIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;
  duk_get_prop_string(duk_ctx, nodeIdx, "output");
  int outputIdx = duk_normalize_index(duk_ctx, -1);

  struct timeval tv;
  gettimeofday(&tv, nullptr);
  auto nowMs = tv.tv_sec * 1000 + tv.tv_usec / 1000;

  duk_get_prop_string(duk_ctx, outputIdx, "elapsedMs");
  auto lastFrameMs = duk_get_number(duk_ctx, -1);
  duk_pop(duk_ctx);

  lpduk_set_prop_number(duk_ctx, outputIdx, "frameCounter", lp_ctx->frame_counter);
  lpduk_set_prop_number(duk_ctx, outputIdx, "elapsedMs", nowMs);
  lpduk_set_prop_number(duk_ctx, outputIdx, "lastFrameDurationMs", nowMs - lastFrameMs);

  duk_pop(duk_ctx); // output

  return 0;
}
