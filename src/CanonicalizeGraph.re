open Definition;
open OutputOrdering;
open NodeInputOrdering;

let rec findIndexExn: 'a .(Belt.List.t('a), 'a) => int =
  (list, needle) =>
    switch (list) {
    | [] => raise(Not_found)
    | [head, ...rest] => head == needle ? 0 : 1 + findIndexExn(rest, needle)
    };

let canonicalizeConnectionSide =
    (
      graph: graphImplementation,
      dependencies: publishingDependencies,
      nodeOrdering: Belt.List.t(nodeID),
      graphNibOrdering: Belt.List.t(nodeID),
      connectionSide: connectionSide,
      isSink: bool,
    )
    : publishingConnectionSide =>
  switch (connectionSide.node) {
  | GraphConnection => {
      node: PublishingGraphConnection,
      nib:
        switch (connectionSide.nib) {
        | ValueConnection => raise(InvalidConnection)
        | PositionalConnection(_) => raise(InvalidConnection)
        | NibConnection(nibID) =>
          PublishingNibConnection(findIndexExn(graphNibOrdering, nibID))
        },
    }
  | NodeConnection(nodeID) => {
      node: PublishingNodeConnection(findIndexExn(nodeOrdering, nodeID)),
      nib:
        switch (connectionSide.nib) {
        | ValueConnection =>
          isSink ? raise(InvalidConnection) : PublishingValueConnection
        | PositionalConnection(index) =>
          isSink ? PublishingNibConnection(index) : raise(InvalidConnection)
        | NibConnection(nibID) =>
          switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
          | ReferenceNode => raise(InvalidConnection)
          | ListNode(_) => raise(InvalidConnection)
          | DefinedNode({definitionID}) =>
            let dependency =
              Belt.Map.String.getExn(dependencies, definitionID);
            PublishingNibConnection(
              findIndexExn(
                isSink ? dependency.inputOrdering : dependency.outputOrdering,
                nibID,
              ),
            );
          }
        },
    }
  };

let canonicalizeGraph =
    (graph: graphImplementation, dependencies: publishingDependencies)
    : publishingGraphImplementation => {
  let outputOrdering = getOutputOrdering(graph, dependencies);
  let (nodeOrdering, inputOrdering) =
    getNodeInputOrdering(graph, dependencies, outputOrdering);

  {
    inputCount: Belt.List.size(inputOrdering),
    outputCount: Belt.List.size(outputOrdering),
    nodes:
      Belt.List.map(nodeOrdering, nodeID =>
        switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
        | ReferenceNode => PublishingReferenceNode
        | ListNode(length) => PublishingListNode(length)
        | DefinedNode({kind, definitionID}) =>
          PublishingDefinedNode({
            kind,
            contentID:
              Belt.Map.String.getExn(dependencies, definitionID).contentID,
          })
        }
      ),
    connections:
      Belt.List.sort(
        Belt.List.map(Belt.Map.toList(graph.connections), ((sink, source)) =>
          {
            sink:
              canonicalizeConnectionSide(
                graph,
                dependencies,
                nodeOrdering,
                outputOrdering,
                sink,
                true,
              ),
            source:
              canonicalizeConnectionSide(
                graph,
                dependencies,
                nodeOrdering,
                inputOrdering,
                source,
                false,
              ),
          }
        ),
        compare,
      ),
  };
};
