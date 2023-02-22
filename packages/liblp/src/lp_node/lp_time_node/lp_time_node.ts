import { lp_node_instance } from "../lp_node";

export const lp_time_node_template = {
  "objType": "node_instance",
  "nodeType": "time",
  "input": {},
  "output": {
    "frameCounter": 0,
    "elapsedMs": 0,
    "lastFrameDurationMs": 0
  },
  "config": {},
  "state": {},
  "connections": {}
} satisfies lp_node_instance<"time">;
