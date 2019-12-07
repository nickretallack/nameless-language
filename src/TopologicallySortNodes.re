let rec f =
        (
          nodes: list(NodeWithID.t),
          connections: ConnectionMap.t,
          scopes: NodeScopeSet.t,
        )
        : list(list(NodeWithID.t)) => {
  let (availableNodes, unavailableNodes) =
    Belt.List.partition(nodes, node =>
      IsRootNode.f(node, connections, scopes)
    );
  let remainingConnections =
    Belt.Map.keep(connections, (sink, _source) =>
      switch (sink.node) {
      | GraphConnection => false
      | NodeConnection(nodeID) =>
        !Belt.List.some(availableNodes, node => node.id == nodeID)
      }
    );
  let newScopes =
    Belt.List.reduce(
      availableNodes, scopes, (acc: NodeScopeSet.t, node: NodeWithID.t) =>
      switch (node.node.kind) {
      | DefinedNode({kind: FunctionDefinitionNode}) =>
        Belt.Set.add(acc, NodeScope(node.id))
      | _ => acc
      }
    );
  if (Belt.List.length(unavailableNodes) == 0) {
    [availableNodes];
  } else {
    if (Belt.List.length(nodes) == Belt.List.length(unavailableNodes)) {
      raise(Exception.CycleDetected);
    };
    [
      availableNodes,
      ...f(unavailableNodes, remainingConnections, newScopes),
    ];
  };
};
