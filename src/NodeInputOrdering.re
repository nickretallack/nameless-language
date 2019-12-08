let rec visitConnection =
        (
          graph: GraphImplementation.t,
          dependencies: PublishingDependencyMap.t,
          connectionSide: ConnectionSide.t,
          nodes: list(NodeID.t),
        )
        : list(NodeID.t) =>
  switch (Belt.Map.get(graph.connections, connectionSide)) {
  | None => nodes
  | Some({node}) =>
    switch (node) {
    | GraphConnection => nodes
    | NodeConnection(nodeID) =>
      if (ListHas.f(nodes, nodeID)) {
        nodes;
      } else {
        let nodeAcc = [nodeID, ...nodes];
        switch (Belt.Map.String.getExn(graph.nodes, nodeID).kind) {
        | ReferenceNode => nodeAcc
        | ListNode(length) =>
          Belt.List.reduce(
            Belt.List.makeBy(length, Identity.f),
            nodeAcc,
            (nodes: list(NodeID.t), index: int) =>
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
      graph: GraphImplementation.t,
      dependencies: PublishingDependencyMap.t,
      nodeID: NodeID.t,
      nodes: list(NodeID.t),
    )
    : list(NodeID.t) =>
  visitConnection(
    graph,
    dependencies,
    {node: NodeConnection(nodeID), nib: ValueConnection},
    nodes,
  )
and visitNibConnections =
    (
      graph: GraphImplementation.t,
      dependencies: PublishingDependencyMap.t,
      nodeID: NodeID.t,
      definitionID: DefinitionID.t,
      isInputs: bool,
      nodes: list(NodeID.t),
    )
    : list(NodeID.t) => {
  let dependency = Belt.Map.String.getExn(dependencies, definitionID);
  Belt.List.reduce(
    isInputs ? dependency.inputOrdering : dependency.outputOrdering,
    nodes,
    (acc: list(NodeID.t), nibID: NibID.t) =>
    visitConnection(
      graph,
      dependencies,
      {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
      acc,
    )
  );
};

let f =
    (
      graph: GraphImplementation.t,
      dependencies: PublishingDependencyMap.t,
      outputOrdering: list(NibID.t),
    )
    : list(NodeID.t) =>
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
