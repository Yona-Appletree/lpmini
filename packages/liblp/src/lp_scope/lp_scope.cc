#include "lp_scope.h"
#include "../lp_node/lp_node.h"
#include "../lp_obj/lp_obj.h"
#include "../util/lp_duktape.h"
#include "../lp_connection/lp_connection.h"

#include <unordered_set>
#include <string>

/**
 * Creates a new node scope for the given config and initializes the nodes.
 *
 * Assumes the scope definition is on the stack.
 * Replaces it with the scope instance.
 *
 * @param ctx
 */
int contextIdx(
  lp_context *lp_ctx,
  int contextIdx
) {
  auto duk_ctx = lp_ctx->duk_ctx;

  int scopeIdx = LP_OBJ_ASSERT_STACK(lp_obj_scope_def)

  // [scope_def]
  // Change the object type to "scope_instance"
  lpduk_set_prop_literal(duk_ctx, -1, "objType", lp_obj_scope_instance);
  // [scope_def]

  // Iterate the nodes and create them
  duk_get_prop_string(duk_ctx, -1, "nodes");
  int nodesIdx = duk_normalize_index(duk_ctx, -1);

  // [scope_instance, nodes]
  duk_enum(duk_ctx, nodesIdx, 0);
  // [scope_instance, nodes, enum]
  while (duk_next(duk_ctx, -1, 1)) {
    // [scope_instance, nodes, enum, key, value]
    const char *id = duk_get_string(duk_ctx, -2);

    int nodeDefIdx = duk_normalize_index(duk_ctx, -1);

    // Create the node
    lp_node_create(lp_ctx, id, contextIdx, scopeIdx, nodeDefIdx);
    LP_OBJ_ASSERT_STACK("node_instance")
    // [scope_instance, nodes, enum, key, node_def, node_instance]
    duk_swap_top(duk_ctx, -2);
    duk_pop(duk_ctx);
    // [scope_instance, nodes, enum, key, node_instance]

    // Store the node in the object
    duk_put_prop(duk_ctx, nodesIdx);
    // [scope_instance, nodes, enum]
  }
  // [scope_instance, nodes, enum]
  duk_pop_2(duk_ctx);
  // [scope_instance]

  return 0;
}


int lp_scope_get_node_ids(
  lp_context *lp_ctx,
  std::unordered_set<std::string> &nodeIds
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  LP_OBJ_ASSERT_STACK(lp_obj_scope_instance)

  // Iterate the nodes in the scope
  duk_get_prop_string(duk_ctx, -1, "nodes");
  // [scope_instance, nodes]
  duk_enum(duk_ctx, -1, 0);
  // [scope_instance, nodes, enum]
  while (duk_next(duk_ctx, -1, 0)) {
    // [scope_instance, nodes, enum, nodeId]
    const char *nodeId = duk_get_string(duk_ctx, -1);
    nodeIds.insert(std::string(nodeId));
    duk_pop(duk_ctx);
    // [scope_instance, nodes, enum]
  }
  // [scope_instance, nodes, enum]
  duk_pop_2(duk_ctx);
  // [scope_instance]
}


static const int scopeEvalIterationLimit = 100;

int lp_scope_eval(
  lp_context *lp_ctx,
  int contextIdx,
  int scopeIdx
) {
  duk_context *duk_ctx = lp_ctx->duk_ctx;

  // Create a pending set of nodes to evaluate

  // for up to iteration limit
  //   keep a list of nodes evaluated this iteration
  //   Iterate the nodes:
  //    - iterate the node connections
  //      - if the connection is for a node that is in the pending set, skip this node
  //    - evaluate the node
  //    - add it to the evaluatedThisIteration
  //   if evaluatedThisIteration is empty, error
  //   remove all entries from evaluatedThisIteration from the pending set

  // create a hashmap of string to boolean
  std::unordered_set<std::string> pendingNodes;

  lp_scope_get_node_ids(lp_ctx, pendingNodes);

  if (pendingNodes.empty()) {
    return 0;
  }

  // Get the nodes
  duk_get_prop_string(duk_ctx, -1, "nodes");
  // [scope_instance, nodes]

  std::unordered_set<std::string> nodeIdsEvaluatedThisIteration;
  for (int iterationIndex = 0; iterationIndex < scopeEvalIterationLimit; iterationIndex++) {
    nodeIdsEvaluatedThisIteration.clear();

    if (pendingNodes.empty()) {
      break;
    }

    for (auto nodeId: pendingNodes) {
      duk_get_prop_string(duk_ctx, -1, nodeId.c_str());
      // [scope_instance, nodes, node_instance]

      int nodeIdx = LP_OBJ_ASSERT_STACK(lp_obj_node_instance)

      // Iterate the node connections
      duk_get_prop_string(duk_ctx, nodeIdx, "connections");

      duk_enum(duk_ctx, -1, 0);

      bool allDependenciesEvaluated = true;

      while (duk_next(duk_ctx, -1, 1)) {
        int connectionIdx = LP_OBJ_ASSERT_STACK(lp_obj_connection_instance)

        // [scope_instance, nodes, node_instance, connections, enum, connection_id, connection]
        duk_get_prop_string(duk_ctx, -2, "nodeId");
        const char *nodeId = duk_get_string(duk_ctx, -1);
        duk_pop(duk_ctx);

        if (pendingNodes.find(nodeId) != pendingNodes.end()) {
          allDependenciesEvaluated = false;
          duk_pop_2(duk_ctx);
          break;
        } else {
          // push the node instance
          lp_connection_apply(
            lp_ctx,
            scopeIdx,
            nodeIdx,
            duk_get_top(duk_ctx) - 1
          );

          // [scope_instance, nodes, node_instance, connections, enum, connection_id, connection]
          duk_pop_2(duk_ctx);
        }
      }

      // [scope_instance, nodes, node_instance, connections, enum]
      duk_pop_2(duk_ctx);
      // [scope_instance, nodes, node_instance]

      if (allDependenciesEvaluated) {
        // Evaluate the node
        lp_node_eval(lp_ctx, contextIdx, scopeIdx, nodeIdx);

        nodeIdsEvaluatedThisIteration.insert(nodeId);
      }

      duk_pop(duk_ctx);
    }

    if (nodeIdsEvaluatedThisIteration.empty()) {
      duk_error(
        duk_ctx,
        DUK_ERR_TYPE_ERROR,
        "No new nodes were evaluated in scope eval iteration. This is likely due to a circular dependency."
      );
    }
  }

  duk_pop(duk_ctx);

  return 0;
}
