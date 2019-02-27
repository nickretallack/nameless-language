open Definition;

let getNibPosition =
    (
      connectionSide: connectionSide,
      isSink: bool,
      nodeLayouts: Belt.Map.String.t(LayoutGraph.nodeLayout),
      nodes: nodes,
      definition: definition,
      graphWidth: int,
      definitions: definitions,
    )
    : LayoutGraph.nodePosition =>
  switch (connectionSide.node) {
  | GraphConnection =>
    let nibIndex =
      Helpers.findByIndexExn(
        isSink ?
          definition.display.outputOrdering : definition.display.inputOrdering,
        nibID =>
        NibConnection(nibID) == connectionSide.nib
      );
    {columns: isSink ? (-1) : graphWidth - 2, rows: nibIndex + 1};
  | NodeConnection(nodeID) =>
    let nodePosition = Belt.Map.String.getExn(nodeLayouts, nodeID).position;
    let nibIndex =
      getNodeNibIndex(
        Belt.Map.String.getExn(nodes, nodeID),
        definitions,
        connectionSide.nib,
        isSink,
      );
    {columns: nodePosition.columns, rows: nodePosition.rows + nibIndex};
  };

let rec getParentScopes = (nodeID: nodeID, nodes: nodes): list(nodeID) => {
  switch (Belt.Map.String.getExn(nodes, nodeID).scope) {
  | GraphScope => []
  | NodeScope(nodeID) => [nodeID, ...getParentScopes(nodeID, nodes)]
  };
};

let collisionDetect =
    (
      nodeLayout: LayoutGraph.nodeLayout,
      nodePosition: LayoutGraph.nodePosition,
    )
    : bool =>
  nodePosition.columns >= nodeLayout.position.columns
  && nodePosition.columns < nodeLayout.position.columns
  + nodeLayout.size.columns
  && nodePosition.rows >= nodeLayout.position.rows
  && nodePosition.rows <= nodeLayout.position.rows
  + nodeLayout.size.rows;

let calculate =
    (
      nodeLayouts: Belt.Map.String.t(LayoutGraph.nodeLayout),
      connections: connections,
      nodes: nodes,
      definition: definition,
      definitions: definitions,
      graphWidth: int,
    )
    : Belt.Map.t(connectionSide, list(int), ConnectionComparator.identity) => {
  Belt.Map.mapWithKey(
    connections,
    (sink, source) => {
      let startPosition =
        getNibPosition(
          sink,
          true,
          nodeLayouts,
          nodes,
          definition,
          graphWidth,
          definitions,
        );
      let endPosition =
        getNibPosition(
          source,
          false,
          nodeLayouts,
          nodes,
          definition,
          graphWidth,
          definitions,
        );
      let length = endPosition.columns - startPosition.columns - 1;
      let parentScopes =
        Belt.Set.String.fromArray(
          Belt.List.toArray(
            switch (sink.node) {
            | GraphConnection => []
            | NodeConnection(nodeID) => getParentScopes(nodeID, nodes)
            },
          ),
        );
      // starts at the source.
      // ignores all parent scopes of the sink
      // collision detects with all other nodes at each iteration
      // can use color index as a slight vertical offset later
      Belt.List.reverse(
        fst(
          Belt.List.reduce(
            Belt.List.makeBy(length, index => index),
            ([], endPosition.rows),
            ((results, rows), index) => {
              let columns = endPosition.columns - index - 1;
              let position: LayoutGraph.nodePosition = {columns, rows};
              let collisions =
                Belt.Map.String.keep(nodeLayouts, (nodeID, layout) =>
                  !Belt.Set.String.has(parentScopes, nodeID)
                  && collisionDetect(layout, position)
                );
              let rows =
                if (Belt.Map.String.isEmpty(collisions)) {
                  rows;
                } else {
                  let collisions =
                    Belt.List.fromArray(
                      Belt.Map.String.valuesToArray(collisions),
                    );
                  let outermostCollision =
                    Belt.List.reduce(
                      List.tl(collisions),
                      List.hd(collisions),
                      (acc, layout) =>
                      layout.depth > acc.depth ? layout : acc
                    );
                  let bottom =
                    outermostCollision.position.rows
                    + outermostCollision.size.rows;
                  if (abs(outermostCollision.position.rows - rows)
                      < abs(bottom - rows)) {
                    outermostCollision.position.rows - 1;
                  } else {
                    bottom;
                  };
                };
              ([rows, ...results], rows);
            },
          ),
        ),
      );
    },
  );
};
