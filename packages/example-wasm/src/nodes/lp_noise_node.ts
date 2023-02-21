import { lp_node } from "./lp_node";

export interface lp_noise_node extends lp_node {
  type: "noise",
  "input": {
    "pos": [ number, number, number ],
    "octaves": number,
    "scale": number
  },
  "output": number
}
