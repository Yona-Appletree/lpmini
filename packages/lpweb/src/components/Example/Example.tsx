import LpContext from "@emscripten-cplusplus-webpack-example/example-wasm/src/LpContext";
import { lp_noise_node } from "@emscripten-cplusplus-webpack-example/example-wasm/src/nodes/lp_noise_node";
import React, { useEffect, useState } from "react"

export function Example() {
  const [ result, setResult ] = useState<string | null>(null)

  useEffect(
    () => {
      (async () => {
        const context = await LpContext.create(
          {},
          {}
        )

        const noiseNode = context.addNode<lp_noise_node>(
          "noise"
        )

        const result = String(noiseNode.evaluate({
          pos: [ 0.5, 0.25, 0.123 ],
        }));

        setResult(result);
      })();
    },
    []
  )

  return <div>
    <h1>
      LPMini
    </h1>
    {(() => {
      switch (result) {
        case null: {
          return <p>fib(20): loading</p>
        }
        default: {
          return <p>fib(20): {result}. it works!!</p>
        }
      }
    })()}
  </div>
}
