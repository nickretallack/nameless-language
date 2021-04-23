let rec f = (parent: NodeID.t, child: NodeID.t, nodes: NodeMap.t): bool =>
  if parent == child {
    true
  } else {
    switch Belt.Map.String.getExn(nodes, child).scope {
    | GraphScope => false
    | NodeScope(nodeID) => f(parent, nodeID, nodes)
    }
  }
