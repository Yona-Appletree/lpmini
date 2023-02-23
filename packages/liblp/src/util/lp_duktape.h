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

void lpduk_set_prop_literal(
  duk_context *duk_ctx,
  int index,
  const char *name,
  const char *value
);

void lpduk_set_prop_empty_object(
  duk_context *duk_ctx,
  int index,
  const char *name
);

void lpduk_set_prop_number(
  duk_context *duk_ctx,
  int index,
  const char *name,
  double number
);


/**
 * Resolves a path in a given stack object.
 *
 * The path is an array of strings or numbers.
 * The root object is the object on the top of the stack.
 * The result object is left on the top of the stack.
 *
 * An empty array, or other value, will result in the root object.
 *
 */
void lpduk_get_path(
  duk_context *duk_ctx,
  int rootIdx,
  int pathArrayIdx
);

void lpduk_set_path(
  duk_context *duk_ctx,
  int rootIdx,
  int firstPropIndex,
  int pathArrayIdx,
  int valueIdx
);

/**
 * Converts a Duktape value on the stack to a JSON string, leaving it on the top of the stack.
 *
 * @param duk_ctx
 * @param valueIdx Index of the value on the stack to convert
 * @return Normalized index of the JSON string on the stack
 */
int lpduk_to_json(
  duk_context *duk_ctx,
  int valueIdx
);

#endif
