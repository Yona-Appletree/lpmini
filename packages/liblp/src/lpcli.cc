#include "util/noise.h"

#include "util/lp_duktape.h"
#include "liblp.h"

#include "lp_context/lp_context.h"

#include <stdio.h>
#include <cassert>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"

// =====================================================================================================================
// test_lp_conn_apply

void test_lp_conn_apply() {
  printf("Running test_lp_conn_apply...\n");

  auto lp_ctx = lp_context_create(
    R"(
{
  "objType": "context_def",
  "rootScopeDef": {
    "objType": "scope_def",
    "nodes": {
      "time": {
        "objType": "node_def",
        "nodeType": "time"
      },
      "expr": {
        "objType": "node_def",
        "nodeType": "expr",
        "config": {
          "expr": "x / 10"
        },
        "connections": {
          "c1": {
            "objType": "connection_def",
            "sourceNodeId": "time",
            "inputPath": ["frameCounter"],
            "outputPath": ["x"]
          }
        }
      },
      "noise": {
        "objType": "node_def",
        "nodeType": "noise",
        "connections": {
          "c1": {
            "objType": "connection_def",
            "sourceNodeId": "expr",
            "inputPath": ["value"],
            "outputPath": ["pos", 0]
          }
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

  lp_ctx->frame_counter = 15;
  lp_context_update(lp_ctx);
  lp_context_eval(lp_ctx);

  assert(lp_context_eval_js_number(lp_ctx, "nodes.noise.input.pos[0]") == 1.5);

  lp_context_destroy(lp_ctx);
}


// =====================================================================================================================
// test_lpduk_get_path

void test_lpduk_get_path() {
  printf("Running test_lpduk_get_path...\n");

  duk_context *duk_ctx = duk_create_heap_default();
  duk_eval_string(duk_ctx, "({value:10, child: { cval: 20, cchild: { cval: 30 } }, other:20, ary:[1,2,3] })");
  int targetIdx = duk_normalize_index(duk_ctx, -1);

  int targetJsonIdx = lpduk_to_json(duk_ctx, targetIdx);

  duk_eval_string(duk_ctx, "['child', 'cval']");
  lpduk_get_path(duk_ctx, targetIdx, duk_normalize_index(duk_ctx, -1));
  assert(duk_get_number(duk_ctx, -1) == 20.0);
  duk_pop(duk_ctx);

  duk_eval_string(duk_ctx, "['ary', 2]");
  lpduk_get_path(duk_ctx, targetIdx, duk_normalize_index(duk_ctx, -1));
  assert(duk_get_number(duk_ctx, -1) == 3.0);
  duk_pop(duk_ctx);

  duk_eval_string(duk_ctx, "['value']");
  lpduk_get_path(duk_ctx, targetIdx, duk_normalize_index(duk_ctx, -1));
  assert(duk_get_number(duk_ctx, -1) == 10.0);
  duk_pop(duk_ctx);

  duk_eval_string(duk_ctx, "[]");
  lpduk_get_path(duk_ctx, targetIdx, duk_normalize_index(duk_ctx, -1));
  duk_json_encode(duk_ctx, -1);
  assert(strcmp(duk_get_string(duk_ctx, targetJsonIdx), duk_get_string(duk_ctx, -1)) == 0);
  duk_pop(duk_ctx);
}


// =====================================================================================================================
// test_lpduk_set_path

void test_lpduk_set_path() {
  printf("Running test_lpduk_set_path...\n");

  duk_context *duk_ctx = duk_create_heap_default();
  duk_eval_string(duk_ctx,
                  "x=({value:10, child: { cval: 20, cchild: { cval: 30 } }, other:20, ary:[1,2,3] }),y={foo:x}");
  int rootIdx = duk_normalize_index(duk_ctx, -1);

  duk_push_string(duk_ctx, "foo");
  int fooIdx = duk_normalize_index(duk_ctx, -1);

  duk_eval_string(duk_ctx, "['child', 'cval']");
  duk_push_number(duk_ctx, 1000);
  lpduk_set_path(
    duk_ctx,
    rootIdx,
    fooIdx,
    duk_normalize_index(duk_ctx, -2),
    duk_normalize_index(duk_ctx, -1)
  );
  duk_eval_string(duk_ctx, "x.child.cval");
  //printf("result: %f\n", duk_get_number(duk_ctx, -1));
  assert(duk_get_number(duk_ctx, -1) == 1000);
}

// =====================================================================================================================
// test_partial_apply

void test_partial_apply() {
  printf("Running test_partial_apply...\n");

  duk_context *duk_ctx = duk_create_heap_default();
  duk_eval_string(duk_ctx, "({value:10, child: { cval: 20, cchild: { cval: 30 } }, other:20, ary:[1,2,3] })");
  int targetIdx = duk_normalize_index(duk_ctx, -1);

  duk_eval_string(duk_ctx, "({other:'lolway', child: { cchild: { cval: 1000000 } }, ary: [9,10]})");
  int sourceIdx = duk_normalize_index(duk_ctx, -1);

  lpduk_apply_partial(duk_ctx, 1, targetIdx, sourceIdx);

  duk_dup(duk_ctx, targetIdx);
  duk_json_encode(duk_ctx, -1);
  //printf("result: %s\n", duk_get_string(duk_ctx, -1));
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
  printf("Running test_simple_context...\n");

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

  lp_context_eval(lp_ctx);
  assert(lp_context_eval_js_number(lp_ctx, "nodes.node0.output") == 0.26941515904);
  
  lp_context_destroy(lp_ctx);
}

// =====================================================================================================================
// JS Eval Test Iterations

void test_js_eval_iterations() {
  printf("Running test_js_eval_iterations...\n");

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

  // print the result
  for (int i = 0; i < 10000; i++) {
    lp_context_eval_js(lp_ctx, "nodes.node0.input.pos[0]");
    lpduk_pop_string(lp_ctx->duk_ctx);
  }

  force_gc(lp_ctx);
  lp_context_destroy(lp_ctx);
}


// =====================================================================================================================
// JS Node Test

void test_js_node() {
  printf("Running test_js_node...\n");

  auto lp_ctx = lp_context_create(
    R"(
{
  "objType": "context_def",
  "rootScopeDef": {
    "objType": "scope_def",
    "nodes": {
      "node0": {
        "objType": "node_def",
        "nodeType": "js",
        "config": {
          "evalJs": "return 42;"
        }
      }
    },
    "connections": {
      "c1": {
        "objType": "connection_def",
        "sourceNodeId": "node0",
        "inputPath": [],
        "outputPath": ["result"]
      }
    },
    "input": {},
    "output": {}
  }
}
)"
  );

  lp_context_eval(lp_ctx);
  assert(lp_context_eval_js_number(lp_ctx, "output.result") == 42);

  lp_context_destroy(lp_ctx);
}

// =====================================================================================================================
// Main

int main() {
  test_lpduk_get_path();
  test_lpduk_set_path();
  test_simple_context();
  test_lp_conn_apply();
  test_js_node();

//  test_js_eval_iterations();

  return 0;
}

#pragma clang diagnostic pop
