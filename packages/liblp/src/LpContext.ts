import { lp_context_def } from "src/lp_context/lp_context";
import { lp_scope_instance } from "src/lp_scope/lp_scope";
import { liblp, liblp_context_ptr, liblp_promise } from "./liblp";
import { lp_node_id, lp_node_instance } from "./lp_node/lp_node";
import { applyDeepPartial, PartialDeep } from "./util/PartialDeep";

export default class LpContext<TContextDef extends lp_context_def> {
  private nodeCount = 0;

  static async create<TContextDef extends lp_context_def>(
    definition: TContextDef
  ) {
    const api = await liblp_promise;

    return new LpContext(
      api,
      api.liblp_context_create(
        JSON.stringify(definition)
      )
    )
  }

  private constructor(
    public api: liblp,
    public contextPtr: liblp_context_ptr
  ) {
  }

  destroy() {
    this.api.liblp_context_destroy(this.contextPtr);
  }

  tick() {
    this.api.liblp_context_tick(this.contextPtr);
  }
}

export class LpScope<TScopeDef extends lp_scope_instance> {

}

export class LpNode<TNodeDef extends lp_node_instance> {
  constructor(
    public lpContext: LpContext<any>,
    private nodeId: lp_node_id
  ) {
  }

  get state() {
    return JSON.parse(this.lpContext.api.liblp_eval_expr_to_json(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}`
    )) as TNodeDef;
  }

  get input() {
    return JSON.parse(this.lpContext.api.liblp_eval_expr_to_json(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}.input`
    ));
  }

  set input(input: TNodeDef["input"]) {
    this.lpContext.api.liblp_eval_expr_to_json(
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

  // evaluate(
  //   inputUpdates: PartialDeep<TNodeDef["input"]>
  // ): TNodeDef["output"] {
  //   this.applyInput(inputUpdates);
  //
  //   this.lpContext.api.liblp_eval_node(
  //     this.lpContext.contextPtr,
  //     this.nodeId
  //   );
  //
  //   return this.state.output;
  // }
}
