#include "util/noise.h"

#include "util/lp_duktape.h"
#include "nodes/lp_node_noise.h"
#include "lpapi.h"

#include <stdio.h>

int main() {
  auto context = lpapi_context_create("{}", "{}");
  lpapi_add_node(context, "noise", "noise");
  lpapi_eval_expr(
    context,
    "nodes.noise.input.pos=[0.5,0.25,0.123]"
  );
  lpapi_eval_node(context, "noise");
  printf("Node: %s\n", lpapi_eval_expr(context, "nodes.noise.output"));

  return 0;
}
