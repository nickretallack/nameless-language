[%%debugger.chrome];
open Definition;

let isRootNode =
    (node: nodeWithID, connections: connections, scopes: nodeScopeSet): bool =>
  Belt.Set.has(scopes, node.node.scope)
  && !
       Belt.Map.some(
         connections, (sink: connectionSide, source: connectionSide) =>
         switch (source.node) {
         | GraphConnection => false
         | NodeConnection(connectionNodeID) =>
           node.id != connectionNodeID ?
             false :
             (
               switch (node.node.kind) {
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
        (
          nodes: list(nodeWithID),
          connections: connections,
          scopes: nodeScopeSet,
        )
        : list(list(nodeWithID)) => {
  let (availableNodes, unavailableNodes) =
    Belt.List.partition(nodes, node => isRootNode(node, connections, scopes));
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
      availableNodes, scopes, (acc: nodeScopeSet, node: nodeWithID) =>
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
      raise(CycleDetected);
    };
    [
      availableNodes,
      ...topoSort(unavailableNodes, remainingConnections, newScopes),
    ];
  };
};

let columnizeNodes =
    (nodes: nodes, connections: connections): list(list(nodeWithID)) =>
  topoSort(
    Belt.List.map(Belt.Map.String.toList(nodes), ((nodeID, node)) =>
      {id: nodeID, node}
    ),
    connections,
    Belt.Set.fromArray([|GraphScope|], ~id=(module ScopeComparator)),
  );
