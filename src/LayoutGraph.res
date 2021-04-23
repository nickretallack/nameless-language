let f = (
  scopedNodeIDs: NodeScopeMap.t<Belt.Set.String.t>,
  columnizedNodes: list<list<NodeWithID.t>>,
  definitions: DefinitionMap.t,
  connections: ConnectionMap.t,
): (Belt.Map.String.t<NodeLayout.t>, NodePosition.t) => {
  let (layout, dimensions) = LayoutDefinition.f(
    GraphScope,
    scopedNodeIDs,
    columnizedNodes,
    definitions,
    connections,
  )
  (
    Belt.Map.String.map(layout, nodeLayout => {
      ...nodeLayout,
      position: {
        ...nodeLayout.position,
        rows: nodeLayout.position.rows + 2,
      },
    }),
    {columns: dimensions.columns + 2, rows: dimensions.rows + 1},
  )
}
