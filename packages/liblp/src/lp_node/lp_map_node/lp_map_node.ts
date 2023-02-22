import { lp_node_instance } from "../lp_node";

export const lp_map_node_template = {
  "objType": "node_instance",
  "nodeType": "map",
  "input": {
    "array": [] as Array<unknown>,
    "mapFn": null as (null | ((input: unknown) => unknown))
  },
  "output": [] as Array<unknown>,
  "config": {},
  "state": {},
  "connections": {}
} satisfies lp_node_instance<"map">;
