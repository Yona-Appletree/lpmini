import { describe, expect, it } from "@jest/globals";
import { liblp_promise } from "../liblp";
import { lp_context_def } from "../lp_context/lp_context";

describe(
  "liblp",
  () => {
    it(
      "can handle a basic test case",
      async () => {
        const lib = await liblp_promise;
        const lpCtx = lib.liblp_context_create(JSON.stringify({
          "objType": "context_def",
          "rootScopeDef": {
            "objType": "scope_def",
            "nodes": {
              "node0": {
                "objType": "node_def",
                "nodeType": "noise",
                "input": {
                  "pos": [ 0.5, 0.2, 0.1 ]
                }
              }
            },
            "connections": {},
            "input": {},
            "output": {}
          }
        } satisfies lp_context_def));

        expect(JSON.parse(lib.liblp_eval_expr_to_json(
          lpCtx,
          "nodes.node0.input.pos"
        )))
          .toEqual([ 0.5, 0.2, 0.1 ])

        lib.liblp_context_destroy(lpCtx);
      }
    );
  }
)
