import { lp_node_instance } from "../lp_node";

export const lp_js_node_template = {
  "objType": "node_instance",
  "nodeType": "js",
  "input": {},
  "output": {},
  "config": {
    "initJs": "",
    "updateJs": "",
    "evalJs": "",
    "destroyJs": ""
  },
  "state": {},
  "connections": {}
} satisfies lp_node_instance<"js">;
