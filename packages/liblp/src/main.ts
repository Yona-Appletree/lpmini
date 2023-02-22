import { lp_context_def } from "./lp_context/lp_context";

const testContext: lp_context_def = {
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
};

console.info(testContext);
