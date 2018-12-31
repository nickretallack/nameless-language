open Definition;

type outputOrderingNode = {
  contentID: string,
  kind: definedNodeKind,
  children: Belt.List.t(outputOrderingTree),
}
and outputOrderingTree =
  | OutputOrderingDisconnected
  | OutputOrderingInput
  | OutputOrderingReference
  | OutputOrderingNode(outputOrderingNode);

let sortBy: 'a .(Belt.List.t('a), 'a => 'b) => Belt.List.t('a) =
  (list, func) =>
    Belt.List.map(
      Belt.List.sort(
        Belt.List.map(list, item => (func(item), item)),
        compare,
      ),
      snd,
    );

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
              (
                canonicalizeOutputConnection(
                  graph,
                  dependencies,
                  {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
                ): outputOrderingTree
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

let getOutputOrdering =
    (graph: graphImplementation, dependencies: publishingDependencies)
    : Belt.List.t(nibID) =>
  Belt.Set.String.size(graph.outputs) <= 1 ?
    Belt.Set.String.toList(graph.outputs) :
    sortBy(
      Belt.Set.String.toList(graph.outputs),
      canonicalizeOutput(graph, dependencies),
    );
