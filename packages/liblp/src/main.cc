#include "util/noise.h"

#include "util/lp_duktape.h"
#include "lp_api.h"

#include "lp_context/lp_context.h"

#include <stdio.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"

int main() {
  auto context = lp_context_create(
    R"(
{
  "objType": "context_def",
  "rootScopeDef": {
    "objType": "scope_def",
    "nodes": {
      "node0": {
        "objType": "node_def",
        "nodeType": "noise"
      }
    },
    "connections": {},
    "input": {},
    "output": {}
  }
}
)"
  );

  lp_context_eval(context);

  return 0;
}

void test_partial_apply() {
  duk_context *duk_ctx = duk_create_heap_default();
  duk_eval_string(duk_ctx, "({value:10, child: { cval: 20, cchild: { cval: 30 } }, other:20, ary:[1,2,3] })");
  int targetIdx = duk_normalize_index(duk_ctx, -1);

  duk_eval_string(duk_ctx, "({other:'lolway', child: { cchild: { cval: 1000000 } }, ary: [9,10]})");
  int sourceIdx = duk_normalize_index(duk_ctx, -1);

  lpduk_apply_partial(duk_ctx, 1, targetIdx, sourceIdx);

  duk_dup(duk_ctx, targetIdx);
  duk_json_encode(duk_ctx, -1);
  printf("result: %s", duk_get_string(duk_ctx, -1));
}

#pragma clang diagnostic pop
