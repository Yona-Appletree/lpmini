import { describe, expect, it } from "@jest/globals";
import LpContext from "src/LpContext";

describe(
  "LpContext",
  () => {
    it(
      "can handle a basic test case",
      async () => {
        const lpContext = await LpContext.create({
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
        });

        expect(lpContext.getNode("node0").input.pos)
          .toEqual([ 0.5, 0.2, 0.1 ])

        lpContext.destroy()
      }
    );
  }
)
