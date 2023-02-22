import { lp_connection_def } from "../lp_connection/lp_connection";
import { lp_obj } from "../lp_obj/lp_obj";

export type lp_node_type_id = "noise" | "time";
export type lp_node_id = string

/**
 * An instance of a node in a scope.
 */
export interface lp_node_instance<
  TNodeType extends lp_node_type_id = lp_node_type_id,
  TInput = unknown,
  TOutput = unknown,
  TConfig = unknown,
  TState = unknown
> extends lp_obj<"node_instance"> {
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
  /**
   * Current internal state of the node.
   */
  state: TState;

  connections: Record<string, lp_connection_def>
}

/**
 * An instance of a node in a scope.
 */
export type lp_node_def<
  TTemplate extends lp_node_instance = lp_node_instance
> = Partial<Omit<TTemplate, "nodeType" | "objType">> & Pick<TTemplate, "nodeType"> & lp_obj<"node_def">;
