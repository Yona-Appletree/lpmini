import { lp_obj } from "../lp_obj/lp_obj";

export interface lp_conn {
  sourceNodeId: string | null
  inputPath: string[]
  outputPath: string[]
}

export type lp_conn_def = lp_obj<"connection_def"> & lp_conn
export type lp_conn_instance = lp_obj<"connection_instance"> & lp_conn
