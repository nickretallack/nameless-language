let rec f = (
  nodeScope: NodeScope.t,
  scopedNodeIDs: NodeScopeMap.t<Belt.Set.String.t>,
  columnizedNodes: list<list<NodeWithID.t>>,
  definitions: DefinitionMap.t,
  connections: ConnectionMap.t,
): (Belt.Map.String.t<NodeLayout.t>, NodePosition.t) => {
  let childNodeIDs = Belt.Map.getWithDefault(scopedNodeIDs, nodeScope, Belt.Set.String.empty)
  let (columnFilledness, nodeLayouts) = Belt.List.reduceWithIndex(
    columnizedNodes,
    ([], Belt.Map.String.empty),
    (acc: (array<int>, Belt.Map.String.t<NodeLayout.t>), nodes: list<NodeWithID.t>, columns) =>
      Belt.List.reduce(
        Belt.List.keep(nodes, (node: NodeWithID.t) => Belt.Set.String.has(childNodeIDs, node.id)),
        acc,
        ((columnsFilledness, nodePositions), node: NodeWithID.t) => {
          let (size, children) = switch node.node.kind {
          | DefinedNode({kind: FunctionDefinitionNode}) =>
            layoutSubGraph(node, scopedNodeIDs, columnizedNodes, definitions, connections)
          | _ => (
              {
                open NodePosition
                {
                  rows: NodeCountNibs.f(node.node, definitions) + 1,
                  columns: 1,
                }
              },
              Belt.Map.String.empty,
            )
          }
          let lastColumn = columns + size.columns - 1
          let rows = Belt.Array.reduce(Belt.Array.range(columns, lastColumn), 0, (row, column) =>
            max(row, ArrayGetWithDefault.f(columnsFilledness, column, 0))
          )
          /* Move each subgraph node down into the subgraph */
          let adjustedChildren = Belt.Map.String.map(children, nodeLayout => {
            open NodeLayout
            {
              ...nodeLayout,
              depth: nodeLayout.depth + 1,
              position: {
                ...nodeLayout.position,
                rows: nodeLayout.position.rows + 2 + rows,
              },
            }
          })

          let newFilledness = Belt.Array.makeBy(
            max(Belt.Array.length(columnsFilledness), lastColumn + 1),
            (index: int) =>
              index >= columns && index < columns + size.columns
                ? rows + size.rows + 1
                : ArrayGetWithDefault.f(columnsFilledness, index, 0),
          )
          (
            newFilledness,
            MapMerge.f(
              Belt.Map.String.set(
                nodePositions,
                node.id,
                {
                  size: size,
                  position: {
                    columns: columns,
                    rows: rows,
                  },
                  depth: 1,
                },
              ),
              adjustedChildren,
            ),
          )
        },
      ),
  )
  (
    nodeLayouts,
    {
      rows: Belt.Array.reduce(columnFilledness, 0, max) + 1,
      columns: Belt.Array.length(columnFilledness),
    },
  )
}
and layoutSubGraph = (
  definitionNode: NodeWithID.t,
  scopedNodeIDs: NodeScopeMap.t<Belt.Set.String.t>,
  columnizedNodes: list<list<NodeWithID.t>>,
  definitions: DefinitionMap.t,
  connections: ConnectionMap.t,
) => {
  let firstColumn = ListFindByIndexExn.f(columnizedNodes, nodes =>
    Belt.List.some(nodes, node => node.id == definitionNode.id)
  )

  let nibRows = switch definitionNode.node.kind {
  | DefinedNode({definitionID}) =>
    let display = Belt.Map.String.getExn(definitions, definitionID).display
    max(Belt.Array.length(display.inputOrdering), Belt.Array.length(display.outputOrdering))
  | _ => raise(Not_found)
  }

  let (subLayout, position) = f(
    NodeScope(definitionNode.id),
    scopedNodeIDs,
    columnizedNodes,
    definitions,
    connections,
  )

  (
    {
      /* inline function is at least 2 columns (for the inputs and outputs).
       It expands depending on the subset of the graph that needs to occur inside/beside it. */
      columns: max(0, position.columns - 1 - firstColumn) + 2,
      /* inline function needs to be tall enough to contain its inputs/outputs as well as its subgraph */
      rows: max(nibRows, position.rows) + 1,
    },
    subLayout,
  )
}
