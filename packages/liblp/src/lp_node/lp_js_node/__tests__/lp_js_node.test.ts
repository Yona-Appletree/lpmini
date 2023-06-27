import { describe, expect, it } from "@jest/globals";
import { liblp_promise } from "src/liblp";
import { jsNode } from "src/lp_node/lp_js_node/lp_js_node";
import { scopeDef } from "src/lp_scope/lp_scope";


describe(
  "lp_js_node",
  () => {
    it(
      "can be a simple counter",
      async () => {
        const scope = scopeDef({
          input: { theValue: 10 },
          output: { magicOut: 0 }
        })
        const n = jsNode(
          { offset: 0 },
          0,
          { counter: 0 },
          {
            update: function (state) {
              state.counter++
            },
            eval: function (
              state,
              input
            ) {
              return input.offset + state.counter
            }
          }
        )
        const counterNode = scope.node(n)

        counterNode
          .input(it => it.offset)
          .receiveFrom(scope.input(it => it.theValue))

        counterNode
          .output(it => it)
          .sendTo(scope.output(it => it.magicOut))

        debugger

        const lib = await liblp_promise;
        const ctx = lib.liblp_context_create(scope.toContextJson());

        const tick = () => {
          lib.liblp_context_tick(ctx);
          return JSON.parse(lib.liblp_eval_expr_to_json(
            ctx,
            "output.magicOut"
          ))
        }

        expect(tick())
          .toEqual(1)
        expect(tick())
          .toEqual(2)
      }
    );
  }
)
