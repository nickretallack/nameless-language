let f = (connections: ConnectionMap.t, nodes: NodeMap.t): bool =>
  switch ColumnizeNodes.f(nodes, connections) {
  | exception Exception.CycleDetected => true
  | _ => false
  }
