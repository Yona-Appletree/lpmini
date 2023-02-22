import { lp_api, lp_api_context, lp_api_promise } from "./lp_api";
import { lp_node_def, lp_node_id, lp_node_instance } from "./lp_node/lp_node";
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
      api.lp_api_context_create(
        JSON.stringify(input),
        JSON.stringify(output),
      )
    )
  }

  private constructor(
    public api: lp_api,
    public contextPtr: lp_api_context
  ) {
  }

  destroy() {
    this.api.lp_api_context_destroy(this.contextPtr);
  }

  addNode<T extends lp_node_def>(
    nodeType: T["type"],
    nodeId: lp_node_id = "node" + this.nodeCount++
  ): LpNode<T> {
    this.api.lp_api_add_node(
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
    return JSON.parse(this.api.lp_api_eval_expr(
      this.contextPtr,
      `input`
    )) as TInput;
  }

  set input(input: TInput) {
    this.api.lp_api_eval_expr(
      this.contextPtr,
      "input=" + JSON.stringify(input)
    );
  }

  get output() {
    return JSON.parse(this.api.lp_api_eval_expr(
      this.contextPtr,
      "output"
    )) as TOutput;
  }
}

export class LpNode<TNodeDef extends lp_node_instance> {
  constructor(
    public lpContext: LpContext<any, any>,
    private nodeId: lp_node_id
  ) {
  }

  get state() {
    return JSON.parse(this.lpContext.api.lp_api_eval_expr(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}`
    )) as TNodeDef;
  }

  get input() {
    return JSON.parse(this.lpContext.api.lp_api_eval_expr(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}.input`
    ));
  }

  set input(input: TNodeDef["input"]) {
    this.lpContext.api.lp_api_eval_expr(
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

    this.lpContext.api.lp_api_eval_node(
      this.lpContext.contextPtr,
      this.nodeId
    );

    return this.state.output;
  }
}
