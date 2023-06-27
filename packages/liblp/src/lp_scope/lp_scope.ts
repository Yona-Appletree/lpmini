import {
  connect,
  lp_conn_def,
  lpObjectPathFrom,
  ScopeInput,
  ScopeInputPathBuilder,
  ScopeOutput,
  ScopeOutputPathBuilder
} from "src/lp_conn/lp_conn";
import { lp_context } from "src/lp_context/lp_context";
import { typedPath } from "typed-path";
import { lp_node, lp_node_def, lp_node_id, lp_node_instance, LpNodeDef } from "../lp_node/lp_node";
import { lp_obj } from "../lp_obj/lp_obj";

/**
 * Definition of a scope.
 */
export interface lp_scope_def extends lp_obj<"scope_def"> {
  nodes: Record<lp_node_id, lp_node_def>;
  connections: Record<string, lp_conn_def>;
  input: Record<string, any>;
  output: Record<string, any>;
}

export function lp_scope<TDef extends Partial<lp_scope_def>>(def: TDef): lp_scope_def & TDef {
  return {
    objType: "scope_def",
    nodes: {},
    connections: {},
    input: {},
    output: {},

    ...def,
  } satisfies lp_scope_def & TDef;
}

export function scopeDef<
  TInput extends Record<string, any> = {},
  TOutput extends Record<string, any> = {}
>(def: {
  input?: TInput,
  output?: TOutput,
}) {
  return new LpScopeDef<TInput, TOutput>({
    input: {} as TInput,
    output: {} as TOutput,
    nodes: {},
    connections: {},
    ...def,
  });
}

export class LpScopeDef<
  TInput extends Record<string, any> = {},
  TOutput extends Record<string, any> = {}
> {
  constructor(
    public readonly def: {
      input: TInput,
      output: TOutput,
      nodes: Record<string, LpNodeDef>,
      connections: Record<string, lp_conn_def>,
    }
  ) {
  }

  get nodes() {
    return this.def.nodes
  }

  get connections() {
    return this.def.connections
  }

  get _input(): TInput {
    return this.def.input
  }

  get _output(): TOutput {
    return this.def.output
  }

  node<TNode extends lp_node>(
    def: TNode,
    nodeId: string = `node${Object.keys(this.def.nodes).length}`
  ): LpNodeDef<this, TNode> {
    const node = new LpNodeDef<this, TNode>(
      this,
      nodeId,
      def
    )
    this.nodes[nodeId] = node
    return node;
  }

  input(builder: ScopeInputPathBuilder<this>): ScopeInput<this> {
    return {
      scope: this,
      inputPath: lpObjectPathFrom(builder(typedPath<this["_input"]>())),
      sendTo(dest) {
        connect(
          this,
          dest
        )
      }
    }
  }

  output(builder: ScopeOutputPathBuilder<this>): ScopeOutput<this> {
    return {
      scope: this,
      outputPath: lpObjectPathFrom(builder(typedPath<this["_output"]>())),

      receiveFrom(source) {
        connect(
          source,
          this
        )
      }
    }
  }

  toDef(): lp_scope_def {
    const nodesRec: Record<string, lp_node_def> = {}
    for (const [ nodeId, nodeDef ] of Object.entries(this.nodes)) {
      nodesRec[nodeId] = nodeDef.toDef()
    }

    return {
      objType: "scope_def",
      nodes: nodesRec,
      connections: this.connections,
      input: this._input,
      output: this._output,
    }
  }

  toContextJson(spaces?: number | string) {
    return JSON.stringify(lp_context(this.toDef()), null, spaces)
  }
}


export interface lp_scope_instance {
  nodes: Record<lp_node_id, lp_node_instance>;
  connections: Record<string, lp_conn_def>;
  input: any;
  output: any;
}
