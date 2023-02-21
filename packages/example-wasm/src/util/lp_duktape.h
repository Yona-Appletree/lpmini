#include "../duktape/duktape.h"
#include "../util/noise.h"

#ifndef LP_DUKTAPE_H
#define LP_DUKTAPE_H

#define PRINT_CONTEXT lpduk_print_context(__FILE_NAME__, __LINE__, ctx);

void lpduk_print_context(
  const char *file,
  int line,
  duk_context *ctx
);

typedef struct {
    double x;
    double y;
    double z;
} lp_vec3;

lp_vec3 lpduk_named_vec3(
  duk_context *ctx,
  const char *name
);

int32_t lpduk_named_int(
  duk_context *ctx,
  const char *name
);

double lpduk_named_double(
  duk_context *ctx,
  const char *name
);

void lpduk_set_named_double(
  duk_context *ctx,
  const char *name,
  double value
);

int lpduk_push_node(
  duk_context *ctx,
  const char *json,
  duk_c_function eval_func
);

void lpduk_create_node(
  duk_context *ctx,
  const char *id,
  duk_c_function node_init_func
);

#endif
