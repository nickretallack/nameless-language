open Definition;
let rec getAffectedNodes = (nodeIDs: Belt.Set.String.t, nodes: nodes) =>
  Belt.Set.String.reduce(nodeIDs, nodeIDs, (acc, nodeID) =>
    if (isFunctionDefinitionNode(Belt.Map.String.getExn(nodes, nodeID))) {
      let children =
        Belt.Set.String.fromArray(
          Belt.Map.String.keysToArray(
            Belt.Map.String.keep(nodes, (_nodeID, node) =>
              switch (node.scope) {
              | NodeScope(scopeNodeID) => scopeNodeID == nodeID
              | _ => false
              }
            ),
          ),
        );
      Belt.Set.String.union(acc, getAffectedNodes(children, nodes));
    } else {
      acc;
    }
  );

let connectionSideInvolvesNodeIDs =
    (connectionSide: connectionSide, nodeIDs: Belt.Set.String.t) =>
  switch (connectionSide.node) {
  | NodeConnection(nodeID) => Belt.Set.String.has(nodeIDs, nodeID)
  | _ => false
  };
