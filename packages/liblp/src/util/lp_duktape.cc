#include "../duktape/duktape.h"
#include "../util/noise.h"
#include "./lp_duktape.h"
#include "../lp_node/lp_node.h"

void lpduk_print_stack(
  const char *file,
  int line,
  duk_context *duk_ctx,
  const char *message
) {
  //duk_push_context_dump(duk_ctx);

  int max_items = 10;

  // Print a nice stack trace
  printf("%s:%02d: %s: -1:", file, line, message);

  // Summary stack
  int stack_size = duk_get_top(duk_ctx);
  for (int i = 0; i < stack_size; i++) {
    int index = -1 - i;
    int type = duk_get_type(duk_ctx, index);

    switch (type) {
      case DUK_TYPE_UNDEFINED:
        printf("undefined");
        break;
      case DUK_TYPE_NULL:
        printf("null");
        break;
      case DUK_TYPE_BOOLEAN:
        printf("bool");
        break;
      case DUK_TYPE_NUMBER:
        printf("num");
        break;
      case DUK_TYPE_STRING:
        printf("str");
        break;
      case DUK_TYPE_OBJECT:
        duk_get_prop_string(duk_ctx, index, "objType");
        if (duk_is_string(duk_ctx, -1)) {
          printf("%s", duk_get_string(duk_ctx, -1));
        } else {
          printf("obj");
        }
        duk_pop(duk_ctx);
        break;
      case DUK_TYPE_BUFFER:
        printf("buf");
        break;
      case DUK_TYPE_POINTER:
        printf("ptr");
        break;
      case DUK_TYPE_LIGHTFUNC:
        printf("lightfunc");
        break;
      default:
        printf("unknown");
        break;
    }
    if (i < stack_size - 1) {
      printf(", ");
    }
  }

  printf("\n");

  // Detailed stack
  for (int i = 0; i < stack_size; i++) {
    if (i >= max_items) {
      printf("    ... %d more\n", stack_size - i);
      break;
    }
    int index = -1 - i;
    duk_dup(duk_ctx, index);
    printf("    %d(%d): %s\n", stack_size + index, index, duk_json_encode(duk_ctx, -1));
    duk_pop(duk_ctx);
  }
}

lp_vec3 lpduk_named_vec3(
  duk_context *duk_ctx,
  const char *name
) {
  lp_vec3 vec3 = {0, 0, 0};

  duk_get_prop_string(duk_ctx, -1, name);

  // Expect an array with up to three numbers
  duk_size_t arrayLen = duk_get_length(duk_ctx, -1);
  for (int i = 0; i < arrayLen && i < 3; i++) {
    duk_get_prop_index(duk_ctx, -1, i);
    ((double *) &vec3)[i] = duk_to_number(duk_ctx, -1);
    duk_pop(duk_ctx);
  }

  duk_pop(duk_ctx);

  return vec3;
}

int32_t lpduk_named_int(
  duk_context *duk_ctx,
  const char *name
) {
  duk_get_prop_string(duk_ctx, -1, name);
  int32_t value = duk_get_int(duk_ctx, -1);
  duk_pop(duk_ctx);

  return value;
}

double lpduk_named_double(
  duk_context *duk_ctx,
  const char *name
) {
  duk_get_prop_string(duk_ctx, -1, name);
  double value = duk_get_number(duk_ctx, -1);
  duk_pop(duk_ctx);

  return value;
}

void lpduk_set_named_double(
  duk_context *duk_ctx,
  const char *name,
  double value
) {
  duk_push_number(duk_ctx, value);
  duk_put_prop_string(duk_ctx, -2, name);
}

/**
 * Recursively applies the properties of the source object to the target object.
 *
 * @param duk_ctx
 * @param targetIdx
 * @param sourceIdx
 */
void lpduk_apply_partial(
  duk_context *duk_ctx,
  int recursionDepth,
  int targetIdx,
  int sourceIdx
) {
  if (recursionDepth > 10) {
    duk_error(
      duk_ctx,
      DUK_ERR_TYPE_ERROR,
      "lpduk_apply_partial: Exceeded maximum recursion depth of 10"
    );

    return;
  }

  if (duk_get_type(duk_ctx, sourceIdx) != DUK_TYPE_OBJECT) {
    duk_error(
      duk_ctx,
      DUK_ERR_TYPE_ERROR,
      "lpduk_apply_partial: Source is not an object"
    );

    return;
  }

  if (duk_get_type(duk_ctx, targetIdx) != DUK_TYPE_OBJECT) {
    duk_error(
      duk_ctx,
      DUK_ERR_TYPE_ERROR,
      "lpduk_apply_partial: Target is not an object"
    );

    return;
  }

  // Iterate over the source object's keys
  duk_enum(duk_ctx, sourceIdx, 0);
  while (duk_next(duk_ctx, -1, 1)) {
    // Get the key
    const char *key = duk_get_string(duk_ctx, -2);

    // Get the value
    int sourceValueIdx = duk_normalize_index(duk_ctx, -1);

    // Get the target's value
    duk_get_prop_string(duk_ctx, targetIdx, key);
    int targetValueIdx = duk_normalize_index(duk_ctx, -1);

    // If the target's value is an object, recurse
    if (duk_get_type(duk_ctx, targetValueIdx) == DUK_TYPE_OBJECT) {
      lpduk_apply_partial(duk_ctx, recursionDepth + 1, targetValueIdx, sourceValueIdx);
    } else {
      // Otherwise, set the value
      duk_dup(duk_ctx, sourceValueIdx);
      duk_put_prop_string(duk_ctx, targetIdx, key);
    }

    // Pop target_value, source_value, key
    duk_pop_3(duk_ctx);
  }
  // Pop the enumerator
  duk_pop(duk_ctx);
}


void lpduk_set_prop_literal(
  duk_context *duk_ctx,
  int index,
  const char *name,
  const char *value
) {
  index = duk_normalize_index(duk_ctx, index);

  duk_push_string(duk_ctx, value);
  duk_put_prop_string(duk_ctx, index, name);
}

void lpduk_set_prop_empty_object(
  duk_context *duk_ctx,
  int index,
  const char *name
) {
  index = duk_normalize_index(duk_ctx, index);

  duk_push_object(duk_ctx);
  duk_put_prop_string(duk_ctx, index, name);
}


void lpduk_set_prop_number(
  duk_context *duk_ctx,
  int index,
  const char *name,
  double number
) {
  index = duk_normalize_index(duk_ctx, index);

  duk_push_number(duk_ctx, number);
  duk_put_prop_string(duk_ctx, index, name);
}

void lpduk_get_path(
  duk_context *duk_ctx,
  int rootIdx,
  int pathArrayIdx
) {
  // Push the root on the top of the stack
  duk_dup(duk_ctx, rootIdx);
  int resultIdx = duk_normalize_index(duk_ctx, -1);

  duk_enum(duk_ctx, pathArrayIdx, 0);
  while (duk_next(duk_ctx, -1, 1)) {
    duk_get_prop(duk_ctx, resultIdx);
    duk_swap_top(duk_ctx, resultIdx);
    duk_pop_2(duk_ctx);
  }
  duk_pop(duk_ctx);
}

void lpduk_set_path(
  duk_context *duk_ctx,
  int rootIdx,
  int pathArrayIdx,
  int valueIdx
) {
  // Push the root on the top of the stack
  duk_dup(duk_ctx, rootIdx);
  int tempIdx = duk_normalize_index(duk_ctx, -1);

  auto length = duk_get_length(duk_ctx, pathArrayIdx);
  for (int i = 0; i < length; i++) {
    duk_get_prop_index(duk_ctx, pathArrayIdx, i);
    if (i == length - 1) {
      duk_dup(duk_ctx, valueIdx);
      duk_put_prop(duk_ctx, tempIdx);
    } else {
      duk_get_prop(duk_ctx, tempIdx);
      duk_swap_top(duk_ctx, tempIdx);
      duk_pop(duk_ctx);
    }
  }
}
