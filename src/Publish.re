open Definition;

/* let getOutputOrdering = (graph : graphImplementation, dependencies: publishingDependencies) : Belt.List.t(nibID) =>
   	Belt.Set.size(graph.outputs) <= 1 ?
   	Belt.Set.toList(graph.outputs)
   : Belt.List.sortU(Belt.Set.toList(graph.outputs), (a, b) => {
   	String.compare(
   		canonicalizeOutputAsString(graph, dependencies, a),
   		canonicalizeOutputAsString(graph, dependencies, b),
   	)
   }) */

let rec canonicalizeOutputConnection =
        (
          graph: graphImplementation,
          dependencies: publishingDependencies,
          connectionSink: connectionSide,
        )
        : outputOrderingTree =>
  switch (Belt.Map.get(graph.connections, connectionSink)) {
  | None => OutputOrderingDisconnected
  | Some({node}) =>
    switch (node) {
    | GraphConnection => OutputOrderingInput
    | NodeConnection(nodeID) =>
      switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
      | Reference => OutputOrderingReference
      | DefinedNode({kind, definitionID}) =>
        let dependency = Belt.Map.String.getExn(dependencies, definitionID);
        OutputOrderingNode({
          kind,
          contentID: dependency.contentID,
          children:
            Belt.List.map(dependency.inputOrdering, (nibID: nibID) =>
              canonicalizeOutputConnection(
                graph,
                dependencies,
                {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
              )
            ),
        });
      }
    }
  };

let canonicalizeOutput =
    (
      graph: graphImplementation,
      dependencies: publishingDependencies,
      nibID: nibID,
    )
    : outputOrderingTree =>
  canonicalizeOutputConnection(
    graph,
    dependencies,
    {node: GraphConnection, nib: NibConnection(nibID)},
  );
