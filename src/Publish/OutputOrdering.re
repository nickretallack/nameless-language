open Definition;

type outputOrderingNode = {
  contentID: string,
  kind: definedNodeKind,
  keywordChildren: Belt.List.t(outputOrderingTree),
  valueChild: option(outputOrderingTree),
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

let rec canonicalizeConnection =
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
          valueChild:
            definedNodeKindHasValueInput(kind) ?
              Some(
                canonicalizeConnection(
                  graph,
                  dependencies,
                  {node: NodeConnection(nodeID), nib: ValueConnection},
                ),
              ) :
              None,
          keywordChildren:
            Belt.List.map(dependency.inputOrdering, (nibID: nibID) =>
              canonicalizeConnection(
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
  canonicalizeConnection(
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
