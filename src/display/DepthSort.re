[%%debugger.chrome];
open Definition;

let rec sortWorker =
        (definitions: nodes, seen: Belt.Set.String.t): list(nodes) => {
  let (availableNodes, unavailableNodes) =
    Belt.Map.String.partition(definitions, (_: nodeID, node: node) =>
      switch (node.scope) {
      | GraphScope => true
      | NodeScope(nodeID) => Belt.Set.String.has(seen, nodeID)
      }
    );
  let newSeen =
    Belt.Set.String.fromArray(Belt.Map.String.keysToArray(availableNodes));
  if (Belt.Map.String.isEmpty(unavailableNodes)) {
    [availableNodes];
  } else {
    [
      availableNodes,
      ...sortWorker(unavailableNodes, Belt.Set.String.union(seen, newSeen)),
    ];
  };
};

let sort = (nodes: nodes) => {
  let definitions =
    Belt.Map.String.keep(nodes, (_: nodeID, node: node) =>
      switch (node.kind) {
      | DefinedNode({kind: FunctionDefinitionNode}) => true
      | _ => false
      }
    );
  Belt.List.reverse(sortWorker(definitions, Belt.Set.String.empty));
};
