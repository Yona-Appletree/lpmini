import {
  connect,
  lp_conn_def,
  lpObjectPathFrom,
  NodeInput,
  NodeInputPathBuilder,
  NodeOutput,
  NodeOutputPathBuilder
} from "src/lp_conn/lp_conn";
import { LpScopeDef } from "src/lp_scope/lp_scope";
import { typedPath } from "typed-path";
import { lp_obj } from "../lp_obj/lp_obj";

export type lp_node_type_id =
  | "expr"
  | "func"
  | "js"
  | "map"
  | "noise"
  | "time"

export type lp_node_id = string

export interface lp_node<
  TNodeType extends lp_node_type_id = lp_node_type_id,
  TInput = unknown,
  TOutput = unknown,
  TConfig = unknown
> {
  /**
   * Type of the node.
   */
  nodeType: TNodeType;
  /**
   * Configuration for the node.
   */
  config: TConfig;
  /**
   * Input to the node.
   */
  input: TInput;
  /**
   * Output from the node
   */
  output: TOutput;

  connections: Record<string, lp_conn_def>
}

/**
 * An instance of a node in a scope.
 */
export type lp_node_instance<
  TNodeType extends lp_node_type_id = lp_node_type_id,
  TInput = unknown,
  TOutput = unknown,
  TConfig = unknown
> = lp_obj<"node_instance"> & lp_node<TNodeType, TInput, TOutput, TConfig>

export type lp_node_def<
  TNode extends lp_node = lp_node
> = lp_obj<"node_def"> & Pick<TNode, "nodeType"> & Partial<Exclude<TNode, "nodeType">>

export class LpNodeDef<
  TScope extends LpScopeDef = LpScopeDef,
  TNode extends lp_node = lp_node
> {
  connections: Array<lp_conn_def> = []

  constructor(
    public readonly scope: TScope,
    public readonly nodeId: string,
    public readonly def: TNode
  ) {
  }

  get _input(): TNode["input"] {
    return this.def.input
  }

  get _output(): TNode["output"] {
    return this.def.input
  }

  input(builder: NodeInputPathBuilder<this>): NodeInput<TScope, this> {
    return {
      node: this,
      inputPath: lpObjectPathFrom(builder(typedPath<this["_input"]>())),

      receiveFrom(source) {
        connect(
          source,
          this
        )
      }
    }
  }

  output(builder: NodeOutputPathBuilder<this>): NodeOutput<TScope, this> {
    return {
      node: this,
      outputPath: lpObjectPathFrom(builder(typedPath<this["_output"]>())),

      sendTo(dest) {
        connect(
          this,
          dest
        )
      }
    }
  }

  toDef(): lp_node_def<TNode> {
    const connRecord: Record<string, lp_conn_def> = {}
    this.connections.forEach((
      conn,
      i
    ) => {
      connRecord[i] = conn
    })

    return {
      objType: "node_def",
      nodeType: this.def.nodeType,
      input: this.input,
      config: this.def.config,
      connections: connRecord,
    } as unknown as lp_node_def<TNode>
  }
}
