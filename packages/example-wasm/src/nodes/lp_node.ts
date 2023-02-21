export interface lp_node<TType extends string = string, TInput = unknown, TOutput = unknown> {
  type: TType
  id: string
  input: TInput
  output: TOutput
}
