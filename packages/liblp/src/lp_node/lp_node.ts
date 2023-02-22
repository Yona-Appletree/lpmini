import { lp_connection_def } from "../lp_connection/lp_connection";
import { lp_obj } from "../lp_obj/lp_obj";
import { PartialDeep } from "../util/PartialDeep";

export type lp_node_type_id = "noise"
export type lp_node_id = string

/**
 * An instance of a node in a scope.
 */
export interface lp_node_type_def<
  TNodeType extends lp_node_type_id = lp_node_type_id,
  TInput = unknown,
  TOutput = unknown,
  TConfig = unknown,
  TState = unknown
> extends lp_obj<"node_type_def"> {
  nodeType: TNodeType,
  input: TInput,
  output: TOutput,
  config: TConfig,
  state: TState,
}

/**
 * An instance of a node in a scope.
 */
export interface lp_node_def<
  TTypeDef extends lp_node_type_def = lp_node_type_def
> extends lp_obj<"node_def"> {
  nodeType: TTypeDef["nodeType"],
  config?: PartialDeep<TTypeDef["config"]>,
  input?: PartialDeep<TTypeDef["input"]>,
  connections?: Record<string, lp_connection_def>
}

/**
 * An instance of a node in a scope.
 */
export interface lp_node_instance<
  TType extends lp_node_type_id = lp_node_type_id,
  TInput = unknown,
  TOutput = unknown
> extends lp_obj<"node_instance"> {
  /**
   * Id of the node in the scope. Must be a valid javascript identifier.
   */
  nodeId: lp_node_id;
  /**
   * Type of the node.
   */
  nodeType: TType;
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
  state: null | unknown;
}
