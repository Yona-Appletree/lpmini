/**
 * Identifier for the various types of javascript objects that are used in lplib.
 */
export type lp_obj_type_id =
  | "context_def"
  | "context_instance"
  | "scope_def"
  | "scope_instance"
  | "node_type_def"
  | "node_def"
  | "node_instance"
  | "connection_def"
  | "connection_instance"


/**
 * Base interface for all lplib objects.
 */
export interface lp_obj<TType extends lp_obj_type_id> {
  objType: TType
}
