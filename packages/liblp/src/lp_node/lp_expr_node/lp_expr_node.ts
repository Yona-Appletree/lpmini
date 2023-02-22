import { lp_node_instance } from "../lp_node";

export const lp_expr_node_template = {
  "objType": "node_instance",
  "nodeType": "expr",
  "input": {
    "x": 0,
    "y": 0,
    "z": 0
  },
  "output": {
    "value": 0,
    "exprFn": null as (null | ((input: unknown) => unknown))
  },
  "config": {
    "expr": "x + y + z"
  },
  "state": {},
  "connections": {}
} satisfies lp_node_instance<"expr">;
