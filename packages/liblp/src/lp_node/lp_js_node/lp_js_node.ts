import { funcToDukString } from "src/util/funcToDukString";
import { lp_node, lp_node_instance } from "../lp_node";

export const lp_js_node_template = {
  "objType": "node_instance",
  "nodeType": "js",
  "input": {},
  "output": {},
  "config": {
    "initJs": null as string | null,
    "updateJs": null as string | null,
    "evalJs": null as string | null,
    "destroyJs": null as string | null
  },
  "connections": {}
} satisfies lp_node_instance<"js">;

interface LpJsNodeConfig {
  initJs?: string | null,
  updateJs?: string | null,
  evalJs?: string | null,
  destroyJs?: string | null
}

export function jsNode<TState extends {}, TInput, TOutput>(
  input: TInput,
  output: TOutput,
  state: TState,
  impl: {
    update(state: TState): void,
    eval(
      state: TState,
      input: TInput
    ): TOutput,
    destroy?(state: TState): void
  }
) {
  return {
    nodeType: "js",
    config: {
      initJs: `function(){ return ${JSON.stringify(state)} }`,
      updateJs: impl.update == null ? undefined : funcToDukString(impl.update),
      evalJs: impl.eval == null ? undefined : funcToDukString(impl.eval),
      destroyJs: impl.destroy == null ? undefined : funcToDukString(impl.destroy)
    },
    input: input,
    output: output,
    connections: {}
  } satisfies lp_node<"js", TInput, TOutput, LpJsNodeConfig>;
}

