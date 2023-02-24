#include "duktape/duktape.h"
#include "util/lp_duktape.h"
#include "lp_context/lp_context.h"

extern "C" {
__attribute__((unused))
lp_context *liblp_context_create(
  const char *lp_context_def_json
);

__attribute__((unused))
void liblp_context_destroy(
  lp_context *lp_ctx
);

__attribute__((unused))
void liblp_context_tick(
  lp_context *lp_ctx
);

__attribute__((unused))
const char *liblp_context_to_json(
  lp_context *lp_ctx
);

__attribute__((unused))
const char *liblp_eval_expr_to_json(
  lp_context *lp_ctx,
  const char *expr
);
}
