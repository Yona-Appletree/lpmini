// @ts-ignore
import { lp_api } from "../build/lp_api.js"

export type lpapi_context = number;
export type lpapi_node_type = "noise"
export type lpapi_node_id = string

export interface lpapi {
  lpapi_context_create(
    input_json: string,
    output_json: string,
  ): lpapi_context;

  lpapi_context_destroy(context: lpapi_context): void;

  lpapi_add_node(
    context: lpapi_context,
    node_type: lpapi_node_type,
    node_id: lpapi_node_id
  ): void;

  lpapi_eval_node(
    context: lpapi_context,
    node_id: lpapi_node_id
  ): void;

  lpapi_eval_expr(
    context: lpapi_context,
    expression: string
  ): string;
}

// Since webpack will change the name and potentially the path of the
// `.wasm` file, we have to provide a `locateFile()` hook to redirect
// to the appropriate URL.
// More details: https://kripken.github.io/emscripten-site/docs/api_reference/module.html
export const lp_api_promise: Promise<lpapi> = lp_api({
  // locateFile(path) {
  //   if (path.endsWith(`.wasm`)) {
  //     return fibonacciModule
  //   }
  //   return path
  // },
})
  .then((raw: {
    cwrap: typeof cwrap,
  }) => {
    const lpapi_eval_expr = raw.cwrap(
      "lpapi_eval_expr",
      "string",
      [ "number", "string" ]
    )

    return ({
      raw,

      lpapi_context_create: raw.cwrap(
        "lpapi_context_create",
        "number",
        [ "string", "string" ]
      ),

      lpapi_context_destroy: raw.cwrap(
        "lpapi_context_destroy",
        null,
        [ "number" ]
      ),

      lpapi_add_node: raw.cwrap(
        "lpapi_add_node",
        null,
        [ "number", "string", "string" ]
      ),

      lpapi_eval_node: raw.cwrap(
        "lpapi_eval_node",
        null,
        [ "number", "string" ]
      ),

      lpapi_eval_expr(
        context,
        expression
      ) {
        console.log(
          "lpapi_eval_expr",
          expression
        );
        return lpapi_eval_expr(
          context,
          expression
        );
      }
    } as lpapi);
  });
