#include "duktape/duktape.h"
#include "util/lp_duktape.h"

extern "C" {
duk_context *lp_api_context_create(
  const char *input_json,
  const char *output_json
);

void lp_api_context_destroy(
  duk_context *ctx
);

void lp_api_add_node(
  duk_context *ctx,
  const char *type,
  const char *id
);

void lp_api_eval_node(
  duk_context *ctx,
  const char *id
);

const char *lp_api_eval_expr(
  duk_context *ctx,
  const char *expr
);
}
