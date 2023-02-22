#ifndef LP_OBJ_H
#define LP_OBJ_H

#include "../duktape/duktape.h"
#include "../lp_context/lp_context.h"

static const char *lp_obj_context_def = "context_def";
static const char *lp_obj_context_instance = "context_instance";
static const char *lp_obj_scope_def = "scope_def";
static const char *lp_obj_scope_instance = "scope_instance";
static const char *lp_obj_node_def = "node_def";
static const char *lp_obj_node_instance = "node_instance";
static const char *lp_obj_connection_def = "connection_def";
static const char *lp_obj_connection_instance = "connection_instance";

#define LP_OBJ_ASSERT_STACK(expected_obj_type) \
  lp_obj_assert_stack(__FILE__, __LINE__, lp_ctx, -1, expected_obj_type);

#define LP_OBJ_ASSERT_STACK_INDEX(expected_obj_type, index) \
  lp_obj_assert_stack(__FILE__, __LINE__, lp_ctx, index, expected_obj_type);

int lp_obj_create(
  duk_context *ctx,
  const char *type
);

/**
 * Assert that the top of the stack is an object of the given type.
 *
 * Returns the stack index (from the bottom) of the object.
 *
 * @param lp_ctx
 * @param expected_obj_type
 * @return
 */
int lp_obj_assert_stack(
  const char *filename,
  int line_num,
  lp_context *lp_ctx,
  int index,
  const char *expected_obj_type
);

#endif
