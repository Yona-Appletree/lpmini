#include "duktape/duktape.h"
#include "nodes/lp_node_noise.h"
#include "util/lp_duktape.h"

extern "C" {
duk_context *lpapi_context_create(
  const char *input_json,
  const char *output_json
);

void lpapi_context_destroy(
  duk_context *ctx
);

void lpapi_add_node(
  duk_context *ctx,
  const char *type,
  const char *id
);

void lpapi_eval_node(
  duk_context *ctx,
  const char *id
);

const char *lpapi_eval_expr(
  duk_context *ctx,
  const char *expr
);
}
