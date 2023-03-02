import { lp_context_def } from "src/lp_context/lp_context";
import { lp_scope_def } from "src/lp_scope/lp_scope";
import { liblp, liblp_context_ptr, liblp_promise } from "./liblp";
import { lp_node_def, lp_node_id } from "./lp_node/lp_node";
import { applyDeepPartial, PartialDeep } from "./util/PartialDeep";

export default class LpContext<TContextDef extends lp_context_def> {
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

  evalToJson(expr: string) {
    return this.api.liblp_eval_expr_to_json(
      this.contextPtr,
      expr
    );
  }

  readonly rootScope: LpScope<TContextDef["rootScopeDef"]> = new LpScope(this);
}

export class LpScope<TScopeDef extends lp_scope_def = lp_scope_def> {
  constructor(
    public lpContext: LpContext<any>
  ) {
  }

  private _nodes: Record<lp_node_id, LpNode> = {};
  nodes = new Proxy(
    {},
    {
      get: (
        _,
        nodeId: keyof TScopeDef["nodes"] | symbol
      ) => {
        if (nodeId in this._nodes) {
          return this._nodes[nodeId as lp_node_id];
        }

        const node = new LpNode(
          this,
          nodeId as lp_node_id
        );
        this._nodes[nodeId as lp_node_id] = node;

        return node;
      }
    }
  ) as Record<keyof TScopeDef["nodes"], LpNode>;
}

export class LpNode<TNodeDef extends lp_node_def = lp_node_def> {
  constructor(
    public readonly lpScope: LpScope<any>,
    private nodeId: lp_node_id
  ) {
  }

  private get lpContext() {
    return this.lpScope.lpContext;
  }

  private get api() {
    return this.lpScope.lpContext.api;
  }

  get state() {
    return JSON.parse(this.api.liblp_eval_expr_to_json(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}`
    )) as TNodeDef;
  }

  get input() {
    return JSON.parse(this.api.liblp_eval_expr_to_json(
      this.lpContext.contextPtr,
      `nodes.${this.nodeId}.input`
    ));
  }

  set input(input: TNodeDef["input"]) {
    this.api.liblp_eval_expr_to_json(
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
