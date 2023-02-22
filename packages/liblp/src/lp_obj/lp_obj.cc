#include "lp_obj.h"
#include "../util/lp_duktape.h"

int lp_obj_create(
  duk_context *ctx,
  const char *type
) {
  duk_push_bare_object(ctx);
  duk_push_string(ctx, type);
  duk_put_prop_string(ctx, -2, "objType");
  return 0;
}

int lp_obj_assert_stack(
  const char *filename,
  int line_num,
  lp_context *lp_ctx,
  int index,
  const char *expected_obj_type
) {
  auto duk_ctx = lp_ctx->duk_ctx;

  auto safe_index = index < 0 ? duk_get_top(duk_ctx) + index : index;

  if (expected_obj_type == nullptr && duk_get_top(lp_ctx->duk_ctx) == 0) {
    return safe_index;
  }

  if (duk_is_object(duk_ctx, index)) {
    duk_get_prop_string(duk_ctx, index, "objType");
    const char *actual_obj_type = duk_get_string(duk_ctx, -1);
    duk_pop(duk_ctx);

    if (actual_obj_type == nullptr) {
      actual_obj_type = "<undefined>";
    }

    if (strcmp(actual_obj_type, expected_obj_type) != 0) {
      PRINT_DUK_STACK
      printf(
        "%s:%d Expected stack[%d] to be lp_obj of type %s, got %s",
        filename,
        line_num,
        index,
        expected_obj_type,
        actual_obj_type
      );

      duk_error(
        duk_ctx,
        DUK_ERR_TYPE_ERROR,
        "%s:%d Expected stack[%d] lp_obj of type %s, got %s",
        filename,
        line_num,
        index,
        expected_obj_type,
        actual_obj_type
      );
    } else {
      return safe_index;
    }
  } else {
    const char *actual = duk_get_string(duk_ctx, index);

    if (actual == nullptr) {
      actual = "<null>";
    }

    PRINT_DUK_STACK
    printf(
      "%s:%d Expected stack[%d] lp_obj of type %s; found %s",
      filename,
      line_num,
      index,
      expected_obj_type,
      actual
    );
    duk_error(
      duk_ctx,
      DUK_ERR_TYPE_ERROR,
      "%s:%d Expected stack[%d] object of type %s; found %s",
      filename,
      line_num,
      index,
      expected_obj_type,
      actual
    );
  }
}
