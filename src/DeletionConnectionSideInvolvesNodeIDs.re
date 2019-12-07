let f = (connectionSide: ConnectionSide.t, nodeIDs: Belt.Set.String.t): bool =>
  switch (connectionSide.node) {
  | NodeConnection(nodeID) => Belt.Set.String.has(nodeIDs, nodeID)
  | _ => false
  };
