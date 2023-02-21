/**
 * A connection between two nodes.
 *
 * The first element is the target expression, the second element is the source expression.
 *
 * Essentially an assignment operation:
 * - nodes.input.pos = input.pos;
 * - output.value = nodes.noise.output
 *
 * Note that for now, the source value must statically address nodes. That is, `node.mynode` is allowed,
 * but `node["mynode"]` is not. This is to allow for static analysis of the graph.
 */
export interface lp_connection {
  id: string
  targetExpr: string
  sourceExpr: string
}
