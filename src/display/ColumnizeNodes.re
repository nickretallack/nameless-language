open Definition;

let isRootNode =
    (
      nodeID: nodeID,
      node: node,
      connections: connections,
      scopes: nodeScopeSet,
    )
    : bool =>
  Belt.Set.has(scopes, node.scope)
  && !
       Belt.Map.some(
         connections, (sink: connectionSide, source: connectionSide) =>
         switch (source.node) {
         | GraphConnection => false
         | NodeConnection(connectionNodeID) =>
           nodeID != connectionNodeID ?
             false :
             (
               switch (node.kind) {
               | DefinedNode({kind: FunctionDefinitionNode}) =>
                 switch (source.nib) {
                 | ValueConnection =>
                   switch (sink.node) {
                   | NodeConnection(_) => true
                   | GraphConnection => false
                   }
                 | NibConnection(_) => false
                 | _ => raise(Not_found)
                 }
               | _ =>
                 switch (sink.node) {
                 | NodeConnection(_) => true
                 | GraphConnection => false
                 }
               }
             )
         }
       );

let rec topoSort =
        (nodes: nodes, connections: connections, scopes: nodeScopeSet) => {
  let (availableNodes, unavailableNodes) =
    Belt.Map.String.partition(nodes, (nodeID, _node) =>
      isRootNode(
        nodeID,
        Belt.Map.String.getExn(nodes, nodeID),
        connections,
        scopes,
      )
    );
  let remainingConnections =
    Belt.Map.keep(connections, (sink, _source) =>
      switch (sink.node) {
      | GraphConnection => false
      | NodeConnection(nodeID) =>
        !
          Belt.Map.String.some(availableNodes, (connectionNodeID, _node) =>
            connectionNodeID == nodeID
          )
      }
    );
  let newScopes =
    Belt.Map.String.reduce(
      availableNodes, scopes, (acc: nodeScopeSet, nodeID: nodeID, node: node) =>
      switch (node.kind) {
      | DefinedNode({kind: FunctionDefinitionNode}) =>
        Belt.Set.add(acc, NodeScope(nodeID))
      | _ => acc
      }
    );
  if (Belt.Map.String.isEmpty(unavailableNodes)) {
    [availableNodes];
  } else {
    if (Belt.Map.String.size(nodes) == Belt.Map.String.size(unavailableNodes)) {
      raise(CycleDetected);
    };
    [
      availableNodes,
      ...topoSort(unavailableNodes, remainingConnections, newScopes),
    ];
  };
};

let columnizeNodes = (nodes: nodes, connections: connections) =>
  topoSort(
    nodes,
    connections,
    Belt.Set.fromArray([|GraphScope|], ~id=(module ScopeComparator)),
  );
