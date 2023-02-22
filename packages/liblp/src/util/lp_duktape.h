#include "../duktape/duktape.h"
#include "../util/noise.h"

#ifndef LP_DUKTAPE_H
#define LP_DUKTAPE_H

#define PRINT_DUK_STACK lpduk_print_stack(__FILE_NAME__, __LINE__, duk_ctx, "stack dump");
#define LP_START_FUNC auto start_stack_depth = duk_get_top(duk_ctx);
#define LP_END_FNC \
  auto end_stack_depth = duk_get_top(duk_ctx); \
  if (start_stack_depth != end_stack_depth) { \
    PRINT_DUK_STACK \
    duk_error(duk_ctx, \
              DUK_ERR_TYPE_ERROR, \
              "%s:%d Stack depth changed from %d to %d", \
              __FILE_NAME__, \
              __LINE__, \
              start_stack_depth, \
              end_stack_depth); \
  } \


void lpduk_print_stack(
  const char *file,
  int line,
  duk_context *duk_ctx,
  const char *message
);

typedef struct {
    double x;
    double y;
    double z;
} lp_vec3;

lp_vec3 lpduk_named_vec3(
  duk_context *duk_ctx,
  const char *name
);

int32_t lpduk_named_int(
  duk_context *duk_ctx,
  const char *name
);

double lpduk_named_double(
  duk_context *duk_ctx,
  const char *name
);

void lpduk_set_named_double(
  duk_context *duk_ctx,
  const char *name,
  double value
);

void lpduk_apply_partial(
  duk_context *duk_ctx,
  int recursionDepth,
  int targetIdx,
  int sourceIdx
);

#endif
