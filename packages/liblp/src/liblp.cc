#include "duktape/duktape.h"
#include "util/lp_duktape.h"
#include "lp_context/lp_context.h"
#include "lp_scope/lp_scope.h"


extern "C" {
__attribute__((unused))
lp_context *liblp_context_create(
  const char *lp_context_def_json
) {
  return lp_context_create(lp_context_def_json);
}

__attribute__((unused))
void liblp_context_destroy(
  lp_context *lp_ctx
) {
  lp_context_destroy(lp_ctx);
}

__attribute__((unused))
void liblp_context_tick(
  lp_context *lp_ctx
) {
  lp_context_update(lp_ctx);
  lp_context_eval(lp_ctx);
}

__attribute__((unused))
const char *liblp_context_to_json(
  lp_context *lp_ctx
) {
  lp_context_to_json(lp_ctx);
  return lpduk_pop_string(lp_ctx->duk_ctx);
}

__attribute__((unused))
const char *liblp_eval_expr_to_json(
  lp_context *lp_ctx,
  const char *exp
) {
  //duk_gc(lp_ctx->duk_ctx, 0);

  lp_context_eval_js(lp_ctx, exp);
  duk_json_encode(lp_ctx->duk_ctx, -1);

  return lpduk_pop_string(lp_ctx->duk_ctx);
}
}

