open Definition;
open Helpers;

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
          /* Move each subgraph node down into the subgraph */
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
  (
    Belt.Map.String.map(layout, nodeLayout =>
      {
        ...nodeLayout,
        position: {
          ...nodeLayout.position,
          rows: nodeLayout.position.rows + 1,
        },
      }
    ),
    {columns: dimensions.columns + 2, rows: dimensions.rows},
  );
};
