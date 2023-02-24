import liblpFactory, { EmscriptenModule_liblp } from "../build/liblp";

export type liblp_context_ptr = number;

function wrapModule(module: EmscriptenModule_liblp) {
  return {
    module,

    liblp_context_create:
      module.cwrap(
        "liblp_context_create",
        "number",
        [ "string" ] satisfies Emscripten.JSType[]
      ) as (lp_context_def_json: string) => liblp_context_ptr,

    liblp_context_destroy: module.cwrap(
      "liblp_context_destroy",
      null,
      [ "number" ] satisfies Emscripten.JSType[]
    ) as (lp_ctx: liblp_context_ptr) => void,

    liblp_context_tick: module.cwrap(
      "liblp_context_tick",
      null,
      [ "number" ] satisfies Emscripten.JSType[]
    ) as (lp_ctx: liblp_context_ptr) => void,

    liblp_context_to_json: module.cwrap(
      "liblp_context_to_json",
      "string",
      [ "number" ] satisfies Emscripten.JSType[]
    ) as (lp_ctx: liblp_context_ptr) => string,

    liblp_eval_expr_to_json: module.cwrap(
      "liblp_eval_expr_to_json",
      "string",
      [ "number", "string" ] satisfies Emscripten.JSType[]
    ) as (
      lp_ctx: liblp_context_ptr,
      expr: string
    ) => string,
  }
}

export type liblp = ReturnType<typeof wrapModule>;

// Since webpack will change the name and potentially the path of the
// `.wasm` file, we have to provide a `locateFile()` hook to redirect
// to the appropriate URL.
// More details: https://kripken.github.io/emscripten-site/docs/api_reference/module.html
export const liblp_promise: Promise<liblp> = liblpFactory({
  // locateFile(path) {
  //   if (path.endsWith(`.wasm`)) {
  //     return fibonacciModule
  //   }
  //   return path
  // },
})
  .then(wrapModule);
