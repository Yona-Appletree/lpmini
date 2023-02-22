#ifndef LP_CONTEXT_H
#define LP_CONTEXT_H

#include "../duktape/duktape.h"

typedef struct {
    duk_context *duk_ctx;
    int frame_id;
} lp_context;

lp_context *lp_context_create(
  const char *lp_context_def_json
);

int *lp_context_destroy(
  lp_context *lp_ctx
);

int *lp_context_eval(
  lp_context *lp_ctx
);

#endif
