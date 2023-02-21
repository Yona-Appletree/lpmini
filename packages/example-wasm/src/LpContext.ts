import { lp_api_promise, lpapi, lpapi_context, lpapi_node_id } from "./lpapi";
import { lp_node } from "./nodes/lp_node";
import { applyDeepPartial, PartialDeep } from "./util/PartialDeep";

export default class LpContext<TInput extends object, TOutput extends object> {
  private nodeCount = 0;

  static async create<TInput extends object, TOutput extends object>(
    input: TInput,
    output: TOutput
  ) {
    const api = await lp_api_promise;

    return new LpContext(
      api,
      api.lpapi_context_create(
        JSON.stringify(input),
        JSON.stringify(output),
      )
    )
  }

  private constructor(
    public api: lpapi,
    public contextPtr: lpapi_context
  ) {
  }

  destroy() {
    this.api.lpapi_context_destroy(this.contextPtr);
  }

  addNode<T extends lp_node<any, any, any>>(
    nodeType: T["type"],
    nodeId: lpapi_node_id = "node" + this.nodeCount++
  ): LpNode<T> {
    this.api.lpapi_add_node(
      this.contextPtr,
      nodeType,
      nodeId
    );

    return new LpNode<T>(
      this,
      nodeId
    );
  }

  get input() {
    return JSON.parse(this.api.lpapi_eval_expr(
      this.contextPtr,
      `input`
    )) as TInput;
  }

  set input(input: TInput) {
    this.api.lpapi_eval_expr(
      this.contextPtr,
      "input=" + JSON.stringify(input)
    );
  }

  get output() {
    return JSON.parse(this.api.lpapi_eval_expr(
      this.contextPtr,
      "output"
    )) as TOutput;
  }
}

export class LpNode<TNodeDef extends lp_node> {
  constructor(
    public lpContext: LpContext<any, any>,
    private nodeId: lpapi_node_id
  ) {
  }

  get state() {
    return JSON.parse(this.lpContext.api.lpapi_eval_expr(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}`
    )) as TNodeDef;
  }

  get input() {
    return JSON.parse(this.lpContext.api.lpapi_eval_expr(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}.input`
    ));
  }

  set input(input: TNodeDef["input"]) {
    this.lpContext.api.lpapi_eval_expr(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}.input=` + JSON.stringify(input)
    );
  }

  applyInput(updates: PartialDeep<TNodeDef["input"]>) {
    this.input = applyDeepPartial(
      this.input,
      updates
    );
  }

  evaluate(
    inputUpdates: PartialDeep<TNodeDef["input"]>
  ): TNodeDef["output"] {
    this.applyInput(inputUpdates);

    this.lpContext.api.lpapi_eval_node(
      this.lpContext.contextPtr,
      this.nodeId
    );

    return this.state.output;
  }
}
