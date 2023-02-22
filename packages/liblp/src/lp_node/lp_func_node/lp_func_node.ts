import { lp_scope_def } from "src/lp_scope/lp_scope";
import { lp_node_instance } from "../lp_node";

export const lp_func_node_template = {
  "objType": "node_instance",
  "nodeType": "func",
  "input": {
    "array": [] as Array<unknown>,
  },
  "output": {
    "value": []
  },
  "config": {
    "scope": null as (null | lp_scope_def)
  },
  "state": {},
  "connections": {}
} satisfies lp_node_instance<"func">;
