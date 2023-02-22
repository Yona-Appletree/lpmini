import { lp_node_type_def } from "../lp_node";

export const lp_noise_node_type_def = {
  "objType": "node_type_def",
  "nodeType": "noise",
  "input": {
    "pos": [ 0, 0, 0 ],
    "octaves": 1,
    "scale": 1
  },
  "output": 0,
  "config": {},
  "state": {}
} satisfies lp_node_type_def<"noise">;
