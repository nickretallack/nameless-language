open Definition;

let listHas: 'a. (Belt.List.t('a), 'a) => bool =
  (haystack, needle) => Belt.List.has(haystack, needle, (a, b) => a == b);

let id = x => x;

let rec visitConnection =
        (
          graph: graphImplementation,
          dependencies: publishingDependencies,
          connectionSide: connectionSide,
          nodes: Belt.List.t(nodeID),
        )
        : Belt.List.t(nodeID) =>
  switch (Belt.Map.get(graph.connections, connectionSide)) {
  | None => nodes
  | Some({node}) =>
    switch (node) {
    | GraphConnection => nodes
    | NodeConnection(nodeID) =>
      if (listHas(nodes, nodeID)) {
        nodes;
      } else {
        let nodeAcc = [nodeID, ...nodes];
        switch (Belt.Map.String.getExn(graph.nodes, nodeID).kind) {
        | ReferenceNode => nodeAcc
        | ListNode(length) =>
          Belt.List.reduce(
            Belt.List.makeBy(length, id),
            nodeAcc,
            (nodes: Belt.List.t(nodeID), index: int) =>
            visitConnection(
              graph,
              dependencies,
              {
                node: NodeConnection(nodeID),
                nib: PositionalConnection(index),
              },
              nodes,
            )
          )
        | DefinedNode({kind, definitionID}) =>
          switch (kind) {
          | FunctionCallNode =>
            visitNibConnections(
              graph,
              dependencies,
              nodeID,
              definitionID,
              true,
              nodes,
            )
          | ValueNode => nodes
          | FunctionPointerCallNode =>
            visitNibConnections(
              graph,
              dependencies,
              nodeID,
              definitionID,
              true,
              visitValueConnection(graph, dependencies, nodeID, nodes),
            )
          | FunctionDefinitionNode =>
            visitNibConnections(
              graph,
              dependencies,
              nodeID,
              definitionID,
              false,
              nodes,
            )
          | ConstructorNode =>
            visitNibConnections(
              graph,
              dependencies,
              nodeID,
              definitionID,
              true,
              nodes,
            )
          | AccessorNode =>
            visitValueConnection(graph, dependencies, nodeID, nodes)
          }
        };
      }
    }
  }
and visitValueConnection =
    (
      graph: graphImplementation,
      dependencies: publishingDependencies,
      nodeID: nodeID,
      nodes: Belt.List.t(nodeID),
    )
    : Belt.List.t(nodeID) =>
  visitConnection(
    graph,
    dependencies,
    {node: NodeConnection(nodeID), nib: ValueConnection},
    nodes,
  )
and visitNibConnections =
    (
      graph: graphImplementation,
      dependencies: publishingDependencies,
      nodeID: nodeID,
      definitionID: definitionID,
      isInputs: bool,
      nodes: Belt.List.t(nodeID),
    )
    : Belt.List.t(nodeID) => {
  let dependency = Belt.Map.String.getExn(dependencies, definitionID);
  Belt.List.reduce(
    isInputs ? dependency.inputOrdering : dependency.outputOrdering,
    nodes,
    (acc: Belt.List.t(nodeID), nibID: nibID) =>
    visitConnection(
      graph,
      dependencies,
      {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
      acc,
    )
  );
};

let getNodeInputOrdering =
    (
      graph: graphImplementation,
      dependencies: publishingDependencies,
      outputOrdering: Belt.List.t(nibID),
    )
    : Belt.List.t(nodeID) =>
  Belt.List.reverse(
    Belt.List.reduce(outputOrdering, [], (acc, nibID) =>
      visitConnection(
        graph,
        dependencies,
        {node: GraphConnection, nib: NibConnection(nibID)},
        acc,
      )
    ),
  );
