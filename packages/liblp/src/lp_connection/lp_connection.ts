import { lp_obj } from "../lp_obj/lp_obj";

export interface lp_connection_def extends lp_obj<"connection_def"> {
  nodeId: string | null
  inputPath: string[]
  outputPath: string[]
}

export interface lp_connection_instance extends lp_obj<"connection_instance"> {
  nodeId: string | null
  inputPath: string[]
  outputPath: string[]
}
