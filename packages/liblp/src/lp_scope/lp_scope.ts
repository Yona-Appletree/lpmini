import { lp_conn_def } from "src/lp_conn/lp_conn";
import { lp_node_def, lp_node_id, lp_node_instance } from "../lp_node/lp_node";
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

export interface lp_scope_instance {
  nodes: Record<lp_node_id, lp_node_instance>;
  connections: Record<string, lp_conn_def>;
  input: any;
  output: any;
}
