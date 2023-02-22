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

lp_context *lp_context_create(
  const char *lp_context_def_json
) {
  lp_context *lp_ctx = new lp_context;
  lp_ctx->duk_ctx = duk_create_heap(
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    lp_context_fatal_error_handler
  );
  lp_ctx->frame_id = 0;

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
  lp_scope_create(lp_ctx);
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
