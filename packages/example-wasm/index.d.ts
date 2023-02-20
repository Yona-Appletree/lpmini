/* eslint-disable */
export interface FibWasm {
  _fib(a: number): number;
  _jstest(): number;
  _add(a: number, b: number): number;
}

export declare const FibWasmPromise: Promise<FibWasm>;

export default FibWasmPromise
