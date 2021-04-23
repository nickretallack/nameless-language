let f = (connections: ConnectionMap.t, nodes: NodeMap.t): bool =>
  !Belt.Map.some(connections, (sink, source) => !ConnectionCheckScope.f(source, sink, nodes))
