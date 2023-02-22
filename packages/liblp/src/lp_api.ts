import { liblp } from "../build/liblp";
import { lp_node_id, lp_node_type_id } from "./lp_node/lp_node";

export type lp_api_context = number;

export interface lp_api {
  lp_api_context_create(
    input_json: string,
    output_json: string,
  ): lp_api_context;

  lp_api_context_destroy(context: lp_api_context): void;

  lp_api_add_node(
    context: lp_api_context,
    node_type: lp_node_type_id,
    node_id: lp_node_id
  ): void;

  lp_api_eval_node(
    context: lp_api_context,
    node_id: lp_node_id
  ): void;

  lp_api_eval_expr(
    context: lp_api_context,
    expression: string
  ): string;
}

// Since webpack will change the name and potentially the path of the
// `.wasm` file, we have to provide a `locateFile()` hook to redirect
// to the appropriate URL.
// More details: https://kripken.github.io/emscripten-site/docs/api_reference/module.html
export const lp_api_promise: Promise<lp_api> = liblp({
  // locateFile(path) {
  //   if (path.endsWith(`.wasm`)) {
  //     return fibonacciModule
  //   }
  //   return path
  // },
})
  .then(raw => {
    const lp_api_eval_expr = raw.cwrap(
      "lp_api_eval_expr",
      "string",
      [ "number", "string" ]
    )

    return ({
      raw,

      lp_api_context_create: raw.cwrap(
        "lp_api_context_create",
        "number",
        [ "string", "string" ]
      ),

      lp_api_context_destroy: raw.cwrap(
        "lp_api_context_destroy",
        null,
        [ "number" ]
      ),

      lp_api_add_node: raw.cwrap(
        "lp_api_add_node",
        null,
        [ "number", "string", "string" ]
      ),

      lp_api_eval_node: raw.cwrap(
        "lp_api_eval_node",
        null,
        [ "number", "string" ]
      ),

      lp_api_eval_expr(
        context,
        expression
      ) {
        console.log(
          "lp_api_eval_expr",
          expression
        );
        return lp_api_eval_expr(
          context,
          expression
        );
      }
    } as lp_api);
  });
