#include "util/noise.h"

#include "util/lp_duktape.h"
#include "lp_api.h"

#include "lp_context/lp_context.h"

#include <stdio.h>
#include <cassert>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"

// =====================================================================================================================
// test_lp_connection_apply

void test_lp_connection_apply() {
  auto lp_ctx = lp_context_create(
    R"(
{
  "objType": "context_def",
  "rootScopeDef": {
    "objType": "scope_def",
    "nodes": {
      "time": {
        "objType": "node_def",
        "nodeType": "time",
      },
      "expr": {
        "objType": "node_def",
        "nodeType": "expr",
      },
      "noise": {
        "objType": "node_def",
        "nodeType": "noise",
        "input": {
          "pos": [0.5, 0.2, 0.1]
        },
        "connections": {
          "c1": {
            "sourceNodeId": "time",
            "sourcePath": ["frameCounter"],
            "outputPath": [""]
          }
        }
      },
    },
    "connections": {},
    "input": {},
    "output": {}
  }
}
)"
  );
}


// =====================================================================================================================
// test_lpduk_get_path

void test_lpduk_get_path() {
  duk_context *duk_ctx = duk_create_heap_default();
  duk_eval_string(duk_ctx, "({value:10, child: { cval: 20, cchild: { cval: 30 } }, other:20, ary:[1,2,3] })");
  int targetIdx = duk_normalize_index(duk_ctx, -1);

  duk_eval_string(duk_ctx, "['child', 'cval']");
  lpduk_get_path(duk_ctx, targetIdx, duk_normalize_index(duk_ctx, -1));
  assert(duk_get_number(duk_ctx, -1) == 20.0);

  duk_eval_string(duk_ctx, "['ary', 2]");
  lpduk_get_path(duk_ctx, targetIdx, duk_normalize_index(duk_ctx, -1));
  assert(duk_get_number(duk_ctx, -1) == 3.0);
}


// =====================================================================================================================
// test_lpduk_set_path

void test_lpduk_set_path() {
  duk_context *duk_ctx = duk_create_heap_default();
  duk_eval_string(duk_ctx, "x=({value:10, child: { cval: 20, cchild: { cval: 30 } }, other:20, ary:[1,2,3] })");
  int rootIdx = duk_normalize_index(duk_ctx, -1);

  duk_eval_string(duk_ctx, "['child', 'cval']");
  duk_push_number(duk_ctx, 1000);
  lpduk_set_path(
    duk_ctx,
    rootIdx,
    duk_normalize_index(duk_ctx, -2),
    duk_normalize_index(duk_ctx, -1)
  );
  duk_eval_string(duk_ctx, "x.child.cval");
  printf("result: %f", duk_get_number(duk_ctx, -1));
  assert(duk_get_number(duk_ctx, -1) == 1000);
}

// =====================================================================================================================
// test_partial_apply

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



// =====================================================================================================================
// Basic Test

void force_gc(lp_context *ctx) {
  duk_size_t old_size = ctx->heap_size;
  duk_gc(ctx->duk_ctx, 0);
  duk_gc(ctx->duk_ctx, 0);
  duk_size_t new_size = ctx->heap_size;

  printf("gc: %zu -> %zu\n", old_size, new_size);
}

void test_simple_context() {
  auto lp_ctx = lp_context_create(
    R"(
{
  "objType": "context_def",
  "rootScopeDef": {
    "objType": "scope_def",
    "nodes": {
      "node0": {
        "objType": "node_def",
        "nodeType": "noise",
        "input": {
          "pos": [0.5, 0.2, 0.1]
        }
      }
    },
    "connections": {},
    "input": {},
    "output": {}
  }
}
)"
  );

  force_gc(lp_ctx);

  lp_context_eval(lp_ctx);

  force_gc(lp_ctx);

  // print the result
  printf("%s", lp_context_to_json(lp_ctx));
}


int main() {
  test_lpduk_get_path();
  test_lpduk_set_path();
  test_simple_context();
  test_lp_connection_apply();

  return 0;
}

#pragma clang diagnostic pop
