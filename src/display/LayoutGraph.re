open Definition;
open Helpers;
/*
 Info I need about each node:

 */

let getNodeIDsConnectedToInternalInputs =
    (definitionNodeID: nodeID, connections: connections): Belt.Set.String.t =>
  Belt.Map.reduce(
    connections,
    Belt.Set.String.empty,
    (nodeIDs, sink: connectionSide, source: connectionSide) =>
    switch (source.node) {
    | GraphConnection => nodeIDs
    | NodeConnection(connectionNodeID) =>
      definitionNodeID != connectionNodeID ?
        nodeIDs :
        (
          switch (source.nib) {
          | ValueConnection => nodeIDs
          | NibConnection(_) =>
            switch (sink.node) {
            | GraphConnection => nodeIDs
            | NodeConnection(nodeID) => Belt.Set.String.add(nodeIDs, nodeID)
            }
          | PositionalConnection(_) => raise(Not_found)
          }
        )
    }
  );

let getMaxColumnFromNodeIDs =
    (nodeIDs: Belt.Set.String.t, nodeColumns: Belt.Map.String.t(int)): int =>
  Belt.Set.String.reduce(nodeIDs, 0, (maxColumn, nodeID) =>
    max(maxColumn, Belt.Map.String.getExn(nodeColumns, nodeID))
  );

let getMaxColumn =
    (
      definitionNodeID: nodeID,
      connections: connections,
      childNodeIDs: Belt.Set.String.t,
      nodeColumns: Belt.Map.String.t(int),
    )
    : int =>
  getMaxColumnFromNodeIDs(
    Belt.Set.String.union(
      getNodeIDsConnectedToInternalInputs(definitionNodeID, connections),
      childNodeIDs,
    ),
    nodeColumns,
  );

/* let getMaxRows = (childNodes: nodes, nodeColumns: Belt.Map.String.t(int)): int =>
   Belt.List.reduce(childNodes, Belt.Map.Int.empty, (columns, nodeID: nodeID, node: node) => {
   	Belt.Map.Int.update(columns, Belt.Map.String.get(nodeColumns, nodeID), (maybeSize) =>{
   		let nodeRows =
   		switch(maybeRows){
   			| None => nodeRows
   			| Some(columnRows) => columnRows + nodeRows
   		}
   		}
   	)
   	let column = ;

   }


   ) */

/*
 let sizeSubgraph =
     (
       definitionNode: node,
       definitionNodeID: nodeID,
       connections: connections,
       childNodes: nodes,
       nodeColumns: Belt.Map.String.t(int),
     ) => {
   let firstColumn = Belt.Map.String.getExn(nodeColumns, definitionNodeID);
   let lastColumn =
     getMaxColumn(definitionNodeID, connections, childNodes, nodeColumns);
   let columns = lastColumn - firstColumn;
   Belt.Map.String.reduce(
     childNodes, Belt.Array.make(columns, []), (layout, nodeID, node) =>

   );
 }; */

type nodePosition = {
  columns: int,
  rows: int,
};

type nodeLayout = {
  position: nodePosition,
  size: nodePosition,
};

let layoutSubGraph =
    (
      definitionNode: nodeWithID,
      scopedNodeIDs: nodeScopes(Belt.Set.String.t),
      columnizedNodes: list(list(nodeWithID)),
      definitions: definitions,
      connections: connections,
    ) => {
  let childNodeIDs =
    Belt.Map.getExn(scopedNodeIDs, NodeScope(definitionNode.id));

  let nodeColumns =
    Belt.List.reduceWithIndex(
      columnizedNodes, Belt.Map.String.empty, (acc, nodes, column) =>
      Belt.List.reduce(nodes, acc, (acc, node) =>
        Belt.Map.String.set(acc, node.id, column)
      )
    );

  let firstColumn =
    findByIndexExn(columnizedNodes, nodes =>
      Belt.List.some(nodes, node => node.id == definitionNode.id)
    );

  let lastColumn =
    getMaxColumn(definitionNode.id, connections, childNodeIDs, nodeColumns);

  ({columns: lastColumn - firstColumn, rows: 5}, Belt.Map.String.empty);
  /* TODO: call layoutGraph's body again but with a different scope. */
};

let layoutGraph =
    (
      scopedNodeIDs: nodeScopes(Belt.Set.String.t),
      columnizedNodes: list(list(nodeWithID)),
      definitions: definitions,
      connections: connections,
    )
    : Belt.Map.String.t(nodeLayout) => {
  let childNodeIDs = Belt.Map.getExn(scopedNodeIDs, GraphScope);
  let columnCount = Belt.List.length(columnizedNodes);
  snd(
    Belt.List.reduceWithIndex(
      columnizedNodes,
      (Belt.Array.make(columnCount, 0), Belt.Map.String.empty),
      (
        acc: (array(int), Belt.Map.String.t(nodeLayout)),
        nodes: list(nodeWithID),
        columns,
      ) =>
      Belt.List.reduce(
        Belt.List.keep(nodes, (node: nodeWithID) =>
          Belt.Set.String.has(childNodeIDs, node.id)
        ),
        acc,
        ((columnsFilledness, nodePositions), node: nodeWithID) => {
          let (size, children) =
            switch (node.node.kind) {
            | DefinedNode({kind: FunctionDefinitionNode}) =>
              layoutSubGraph(
                node,
                scopedNodeIDs,
                columnizedNodes,
                definitions,
                connections,
              )
            | _ => (
                {
                  rows: countNodeNibs(node.node, definitions) + 2,
                  columns: 1,
                },
                Belt.Map.String.empty,
              )
            };
          let rows =
            Belt.Array.reduce(
              Belt.Array.range(columns, columns + size.columns - 1),
              0,
              (row, column) =>
              max(row, Belt.Array.getExn(columnsFilledness, column))
            );
          (
            Belt.Array.mapWithIndex(
              columnsFilledness, (index: int, filledness: int) =>
              index >= columns && index < columns + size.columns ?
                rows + size.rows : filledness
            ),
            simpleMergeMaps(
              Belt.Map.String.set(
                nodePositions,
                node.id,
                {
                  size,
                  position: {
                    columns,
                    rows,
                  },
                },
              ),
              children,
            ),
          );
        },
      )
    ),
  );
};
