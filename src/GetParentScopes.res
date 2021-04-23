let rec f = (nodeID: NodeID.t, nodes: NodeMap.t): list<NodeID.t> =>
  switch Belt.Map.String.getExn(nodes, nodeID).scope {
  | GraphScope => list{}
  | NodeScope(nodeID) => list{nodeID, ...f(nodeID, nodes)}
  }
