open Definition;

let listHas: 'a .(Belt.List.t('a), 'a) => bool =
  (haystack, needle) => Belt.List.has(haystack, needle, (a, b) => a == b);

let rec visitConnection =
        (
          graph: graphImplementation,
          dependencies: publishingDependencies,
          connectionSide: connectionSide,
          acc: (Belt.List.t(nodeID), Belt.List.t(nibID)),
        )
        : (Belt.List.t(nodeID), Belt.List.t(nibID)) => {
  let (nodes, inputs) = acc;
  switch (Belt.Map.get(graph.connections, connectionSide)) {
  | None => acc
  | Some({nib, node}) =>
    switch (node) {
    | GraphConnection =>
      switch (nib) {
      | ValueConnection => raise(InvalidConnection)
      | NibConnection(nibID) =>
        listHas(inputs, nibID) ? acc : (nodes, [nibID, ...inputs])
      }
    | NodeConnection(nodeID) =>
      if (listHas(nodes, nodeID)) {
        acc;
      } else {
        let nodeAcc = ([nodeID, ...nodes], inputs);
        switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
        | ReferenceNode => nodeAcc
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
            (acc: (Belt.List.t(nodeID), Belt.List.t(nibID)), nibID: nibID) =>
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
};

let getNodeInputOrdering =
    (
      graph: graphImplementation,
      dependencies: publishingDependencies,
      outputOrder: Belt.List.t(nibID),
    )
    : (Belt.List.t(nodeID), Belt.List.t(nibID)) => {
  let (nodeIDs, nibIDs) =
    Belt.List.reduce(outputOrder, ([], []), (acc, nibID) =>
      visitConnection(
        graph,
        dependencies,
        {node: GraphConnection, nib: NibConnection(nibID)},
        acc,
      )
    );
  (Belt.List.reverse(nodeIDs), Belt.List.reverse(nibIDs));
};
