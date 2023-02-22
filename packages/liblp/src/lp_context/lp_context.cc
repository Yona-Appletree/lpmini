#include "lp_context.h"
#include "../lp_scope/lp_scope.h"
#include "../lp_obj/lp_obj.h"
#include "../util/lp_duktape.h"

//language=json
const char *lp_context_instance_template_json = R"(
{
  "objType": "context_instance",
  "rootScope": null,
  "log": []
}
)";

static void lp_context_fatal_error_handler(
  void *udata,
  const char *msg
) {
  (void) udata;  /* ignored in this case, silence warning */

  /* Note that 'msg' may be NULL. */
  fprintf(stderr, "*** FATAL ERROR: %s\n", (msg ? msg : "no message"));
  fflush(stderr);
  abort();
}


void *lp_context_alloc(
  void *lp_ctx,
  duk_size_t size
) {
  auto result = (duk_size_t *) malloc(size + sizeof(duk_size_t));
  result[0] = size;
  ((lp_context *) lp_ctx)->heap_size += size;
  //printf("alloc: %zu, heap_size: %zu\n", size, ((lp_context *) lp_ctx)->heap_size);
  return result + 1;
}

void *lp_context_realloc(
  void *lp_ctx,
  void *ptr,
  duk_size_t size
) {
  duk_size_t *actual_ptr = (duk_size_t *) ptr - 1;
  auto old_size = actual_ptr[0];

  ((lp_context *) lp_ctx)->heap_size += size - old_size;

  auto *result = (duk_size_t *) realloc(actual_ptr, size + sizeof(duk_size_t));
  result[0] = size;
  //printf("realloc: %zu -> %zu, heap_size: %zu\n", old_size, size, ((lp_context *) lp_ctx)->heap_size);
  return result + 1;
}

void lp_context_free(
  void *lp_ctx,
  void *ptr
) {
  if (ptr == nullptr) {
    return;
  }
  duk_size_t *actual_ptr = (duk_size_t *) ptr - 1;
  auto old_size = actual_ptr[0];

  ((lp_context *) lp_ctx)->heap_size -= old_size;

  //printf("free: %zu, heap_size: %zu\n", old_size, ((lp_context *) lp_ctx)->heap_size);
  free(actual_ptr);
}

lp_context *lp_context_create(
  const char *lp_context_def_json
) {
  lp_context *lp_ctx = new lp_context;
  lp_ctx->duk_ctx = duk_create_heap(
    lp_context_alloc,
    lp_context_realloc,
    lp_context_free,
    lp_ctx,
    lp_context_fatal_error_handler
  );
  lp_ctx->frame_counter = 0;
  lp_ctx->heap_size = 0;

  duk_context *duk_ctx = lp_ctx->duk_ctx;

  // Create the context instance object
  duk_push_string(duk_ctx, lp_context_instance_template_json);
  duk_json_decode(duk_ctx, -1);
  duk_dup(duk_ctx, -1); // dup so it remains on the stack
  duk_put_global_string(duk_ctx, "context");

  int contexIdx = LP_OBJ_ASSERT_STACK("context_instance")

  // Decode the context definition
  duk_push_string(duk_ctx, lp_context_def_json);
  duk_json_decode(duk_ctx, -1);

  int contexDefIdx = LP_OBJ_ASSERT_STACK("context_def")

  // Create the root scope
  duk_get_prop_string(duk_ctx, contexDefIdx, "rootScopeDef");
  lp_scope_create(lp_ctx, contexIdx);
  duk_put_prop_string(duk_ctx, contexIdx, "rootScope");

  duk_pop_2(duk_ctx);

  return lp_ctx;
}

int *lp_context_destroy(
  lp_context *lp_ctx
) {
  auto duk_ctx = lp_ctx->duk_ctx;
  duk_destroy_heap(duk_ctx);
  delete lp_ctx;
  return 0;
}

int *lp_context_eval(
  lp_context *lp_ctx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  duk_get_global_string(duk_ctx, "context");
  int contextIdx = LP_OBJ_ASSERT_STACK(lp_obj_context_instance)

  duk_get_prop_string(duk_ctx, -1, "rootScope");
  int scopeIdx = LP_OBJ_ASSERT_STACK(lp_obj_scope_instance)

  lp_scope_eval(lp_ctx, contextIdx, scopeIdx);
  duk_pop_2(duk_ctx);
}

const char *lp_context_to_json(
  lp_context *lp_ctx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;
  duk_get_global_string(duk_ctx, "context");
  int contextIdx = LP_OBJ_ASSERT_STACK(lp_obj_context_instance);
  duk_json_encode(duk_ctx, contextIdx);
  const char *json = duk_get_string(duk_ctx, -1);
  duk_pop(duk_ctx);
  return json;
}

int lp_context_update(
  lp_context *lp_ctx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  duk_get_global_string(duk_ctx, "context");
  int contextIdx = LP_OBJ_ASSERT_STACK(lp_obj_context_instance);

  duk_get_prop_string(duk_ctx, contextIdx, "rootScope");
  int scopeIdx = LP_OBJ_ASSERT_STACK(lp_obj_scope_instance);

  lp_scope_update(lp_ctx, contextIdx, scopeIdx);
  duk_pop_2(duk_ctx);
}
