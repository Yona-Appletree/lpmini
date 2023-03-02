import { describe, it } from "@jest/globals";
import LpContext from "../LpContext";

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

        // @ts-ignore
        const start = Date.now()
        let errorCount = 0
        const iterationCount = 10000

        for (let i = 0; i < iterationCount; i++) {
//          console.info(`${i}, heap: ${lpContext.api.module.HEAP8.length}`)
          const json = lpContext.evalToJson("nodes")
          try {
            JSON.parse(json)
          } catch (e) {
            throw new Error(`${i}: ${json}`)
          }
        }
        const duration = Date.now() - start

        console.info(lpContext.evalToJson("nodes.node0"))

        console.info(`iterations: ${iterationCount}, duration: ${duration}ms, errorCount: ${errorCount}`)

        lpContext.destroy()
      }
    );
  }
)
