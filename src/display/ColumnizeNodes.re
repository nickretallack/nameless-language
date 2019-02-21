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

let rankConnection =
    (
      sink: connectionSide,
      columns: list(list(nodeWithID)),
      nodeToColumnIndex: Belt.Map.String.t(int),
      definitions: definitions,
      display: display,
    ) => {
  switch (sink.node) {
  | GraphConnection =>
    let sinkColumnIndex = (-1);
    let sinkIndexInColumn = 0;
    let sinkIndex =
      switch (sink.nib) {
      | NibConnection(nibID) =>
        Helpers.findIndexExn(display.outputOrdering, nibID)
      | _ => raise(Not_found)
      };

    (- sinkColumnIndex, sinkIndexInColumn, sinkIndex);
  | NodeConnection(sinkNodeID) =>
    let sinkColumnIndex =
      Belt.Map.String.getExn(nodeToColumnIndex, sinkNodeID); // 1st metric
    let column = Belt.List.getExn(columns, sinkColumnIndex);
    let sinkIndexInColumn =
      Helpers.findByIndexExn(column, node => node.id == sinkNodeID); // 2nd metric

    let node = Belt.List.getExn(column, sinkIndexInColumn);
    let nodeDisplay = displayNode(node.node, definitions, "en");

    let nibCollection =
      isFunctionDefinitionNode(node.node) ?
        nodeDisplay.internalOutputs : nodeDisplay.inputs;
    let sinkIndex =
      Helpers.findByIndexExn(nibCollection, ({nib}) => nib == sink.nib); // 3rd metric
    (- sinkColumnIndex, sinkIndexInColumn, sinkIndex);
  };
};

let sortedColumnizedNodes =
    (
      nodes: nodes,
      connections: connections,
      definitions: definitions,
      display: display,
    )
    : list(list(nodeWithID)) => {
  let columns = columnizeNodes(nodes, connections);
  let nodeToColumnIndex =
    Belt.List.reduceWithIndex(
      columns, Belt.Map.String.empty, (acc, column, index) =>
      Belt.List.reduce(column, acc, (acc, node) =>
        Belt.Map.String.set(acc, node.id, index)
      )
    );
  Belt.List.map(columns, column =>
    Helpers.sortBy(
      column,
      nodeWithID => {
        let relevantConnections =
          Belt.Map.keep(connections, (_sink, source) =>
            switch (source.node) {
            | NodeConnection(nodeID) => nodeID == nodeWithID.id
            | _ => false
            }
          );

        Belt.Map.reduce(
          relevantConnections,
          (max_int, max_int, max_int),
          (acc, sink, _source) => {
            let newRanking =
              rankConnection(
                sink,
                columns,
                nodeToColumnIndex,
                definitions,
                display,
              );
            compare(acc, newRanking) < 0 ? acc : newRanking;
          },
        );
      },
    )
  );
};
