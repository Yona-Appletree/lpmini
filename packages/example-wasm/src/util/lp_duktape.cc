#include "../duktape/duktape.h"
#include "../util/noise.h"
#include "./lp_duktape.h"

void lpduk_print_context(
  const char *file,
  int line,
  duk_context *ctx
) {
  duk_push_context_dump(ctx);

  // Print the dump buffer data
  printf("%s:%02d: Context dump: %s\n", file, line, duk_get_string(ctx, -1));

  duk_pop(ctx);
}


void lpduk_create_node(
  duk_context *ctx,
  const char *id,
  duk_c_function node_init_func
) {
  duk_get_global_string(ctx, "nodes");

  node_init_func(ctx);
  duk_push_string(ctx, id);
  duk_put_prop_string(ctx, -2, "id");
  duk_put_prop_string(ctx, -2, id);
}

lp_vec3 lpduk_named_vec3(
  duk_context *ctx,
  const char *name
) {
  lp_vec3 vec3 = {0, 0, 0};

  duk_get_prop_string(ctx, -1, name);

  // Expect an array with up to three numbers
  duk_size_t arrayLen = duk_get_length(ctx, -1);
  for (int i = 0; i < arrayLen && i < 3; i++) {
    duk_get_prop_index(ctx, -1, i);
    ((double *) &vec3)[i] = duk_to_number(ctx, -1);
    duk_pop(ctx);
  }

  duk_pop(ctx);

  return vec3;
}

int32_t lpduk_named_int(
  duk_context *ctx,
  const char *name
) {
  duk_get_prop_string(ctx, -1, name);
  int32_t value = duk_get_int(ctx, -1);
  duk_pop(ctx);

  return value;
}

double lpduk_named_double(
  duk_context *ctx,
  const char *name
) {
  duk_get_prop_string(ctx, -1, name);
  double value = duk_get_number(ctx, -1);
  duk_pop(ctx);

  return value;
}

void lpduk_set_named_double(
  duk_context *ctx,
  const char *name,
  double value
) {
  duk_push_number(ctx, value);
  duk_put_prop_string(ctx, -2, name);
}

int lpduk_push_node(
  duk_context *ctx,
  const char *json,
  duk_c_function eval_func
) {
  duk_push_string(ctx, json);
  duk_json_decode(ctx, -1);

  duk_push_c_function(ctx, eval_func, 0);
  duk_put_prop_string(ctx, -2, "evaluate");

  return 0;
}
