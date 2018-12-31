open Definition;

let listHas: 'a .(Belt.List.t('a), 'a) => bool =
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
        switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
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
          let valueAcc =
            definedNodeKindHasValueInput(kind) ?
              visitConnection(
                graph,
                dependencies,
                {node: NodeConnection(nodeID), nib: ValueConnection},
                nodeAcc,
              ) :
              nodeAcc;
          Belt.List.reduce(
            Belt.Map.String.getExn(dependencies, definitionID).inputOrdering,
            valueAcc,
            (acc: Belt.List.t(nodeID), nibID: nibID) =>
            visitConnection(
              graph,
              dependencies,
              {node: NodeConnection(nodeID), nib: NibConnection(nibID)},
              acc,
            )
          );
        };
      }
    }
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
