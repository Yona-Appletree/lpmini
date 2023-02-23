import { lp_conn_def } from "src/lp_conn/lp_conn";
import { lp_obj } from "../lp_obj/lp_obj";

export type lp_node_type_id = "noise" | "time";
export type lp_node_id = string

export interface lp_node<
  TNodeType extends lp_node_type_id = lp_node_type_id,
  TInput = unknown,
  TOutput = unknown,
  TConfig = unknown,
  TState = unknown
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
  /**
   * Current internal state of the node.
   */
  state: TState;

  connections: Record<string, lp_conn_def>
}

/**
 * An instance of a node in a scope.
 */
export type lp_node_instance<
  TNodeType extends lp_node_type_id = lp_node_type_id,
  TInput = unknown,
  TOutput = unknown,
  TConfig = unknown,
  TState = unknown
> = lp_obj<"node_instance"> & lp_node<TNodeType, TInput, TOutput, TConfig, TState>

/**
 * An instance of a node in a scope.
 */
export type lp_node_def<
  TTemplate extends lp_node = lp_node
> = lp_obj<"node_instance"> & TTemplate
