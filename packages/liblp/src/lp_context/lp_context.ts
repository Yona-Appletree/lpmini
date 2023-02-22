import { lp_obj } from "../lp_obj/lp_obj";
import { lp_scope_def, lp_scope_instance } from "../lp_scope/lp_scope";

export interface lp_context_def extends lp_obj<"context_def"> {
  rootScopeDef: lp_scope_def;
}

export interface lp_context_instance extends lp_obj<"context_instance"> {
  rootScope: lp_scope_instance;
  log: lp_context_log_entry[];
}

export interface lp_context_log_entry {
  path: string;
  severity: number;
  message: string;
}
