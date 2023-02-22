import { lp_node_instance } from "../lp_node";

export const lp_noise_node_template = {
  "objType": "node_instance",
  "nodeType": "noise",
  "input": {
    "pos": [ 0, 0, 0 ],
    "octaves": 1,
    "scale": 1
  },
  "output": 0,
  "config": {},
  "state": {},
  "connections": {}
} satisfies lp_node_instance<"noise">;
