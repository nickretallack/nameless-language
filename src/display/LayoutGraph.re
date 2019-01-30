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

let rec layoutDefinition =
        (
          nodeScope: nodeScope,
          scopedNodeIDs: nodeScopes(Belt.Set.String.t),
          columnizedNodes: list(list(nodeWithID)),
          definitions: definitions,
          connections: connections,
        )
        : (Belt.Map.String.t(nodeLayout), nodePosition) => {
  let childNodeIDs =
    Belt.Map.getWithDefault(scopedNodeIDs, nodeScope, Belt.Set.String.empty);
  let (columnFilledness, nodeLayouts) =
    Belt.List.reduceWithIndex(
      columnizedNodes,
      ([||], Belt.Map.String.empty),
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
                  rows: countNodeNibs(node.node, definitions) + 1,
                  columns: 1,
                },
                Belt.Map.String.empty,
              )
            };
          let lastColumn = columns + size.columns - 1;
          let rows =
            Belt.Array.reduce(
              Belt.Array.range(columns, lastColumn), 0, (row, column) =>
              max(row, arrayGetWithDefault(columnsFilledness, column, 0))
            );
          /* Move each subgraph node down one row as padding */
          let adjustedChildren =
            Belt.Map.String.map(children, nodeLayout =>
              {
                ...nodeLayout,
                position: {
                  ...nodeLayout.position,
                  rows: nodeLayout.position.rows + 1 + rows,
                },
              }
            );

          let newFilledness =
            Belt.Array.makeBy(
              max(Belt.Array.length(columnsFilledness), lastColumn + 1),
              (index: int) =>
              index >= columns && index < columns + size.columns ?
                rows + size.rows + 1 :
                arrayGetWithDefault(columnsFilledness, index, 0)
            );
          Js.log(lastColumn);
          (
            newFilledness,
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
              adjustedChildren,
            ),
          );
        },
      )
    );
  Js.log2("filledness", columnFilledness);
  (
    nodeLayouts,
    {
      rows: Belt.Array.reduce(columnFilledness, 0, max) + 1,
      columns: Belt.Array.length(columnFilledness),
    },
  );
}
and layoutSubGraph =
    (
      definitionNode: nodeWithID,
      scopedNodeIDs: nodeScopes(Belt.Set.String.t),
      columnizedNodes: list(list(nodeWithID)),
      definitions: definitions,
      connections: connections,
    ) => {
  let firstColumn =
    findByIndexExn(columnizedNodes, nodes =>
      Belt.List.some(nodes, node => node.id == definitionNode.id)
    );

  let nibRows =
    switch (definitionNode.node.kind) {
    | DefinedNode({definitionID}) =>
      let display = Belt.Map.String.getExn(definitions, definitionID).display;
      max(
        Belt.List.length(display.inputOrdering),
        Belt.List.length(display.outputOrdering),
      );
    | _ => raise(Not_found)
    };

  let (subLayout, position) =
    layoutDefinition(
      NodeScope(definitionNode.id),
      scopedNodeIDs,
      columnizedNodes,
      definitions,
      connections,
    );

  (
    {
      /* inline function is at least 2 columns (for the inputs and outputs).
         It expands depending on the subset of the graph that needs to occur inside/beside it. */
      columns: max(0, position.columns - 1 - firstColumn) + 2,
      /* inline function needs to be tall enough to contain its inputs/outputs as well as its subgraph */
      rows: max(nibRows, position.rows),
    },
    subLayout,
  );
};

let layoutGraph =
    (
      scopedNodeIDs: nodeScopes(Belt.Set.String.t),
      columnizedNodes: list(list(nodeWithID)),
      definitions: definitions,
      connections: connections,
    )
    : (Belt.Map.String.t(nodeLayout), nodePosition) => {
  let (layout, dimensions) =
    layoutDefinition(
      GraphScope,
      scopedNodeIDs,
      columnizedNodes,
      definitions,
      connections,
    );
  (layout, {columns: dimensions.columns + 2, rows: dimensions.rows + 1});
};
