let f = (
  connectionSide: ConnectionSide.t,
  isSink: bool,
  nodeLayouts: Belt.Map.String.t<NodeLayout.t>,
  nodes: NodeMap.t,
  definition: Definition.t,
  graphWidth: int,
  definitions: DefinitionMap.t,
): NodePosition.t =>
  switch connectionSide.node {
  | GraphConnection =>
    let nibIndex = ArrayFindIndexByExn.f(
      isSink ? definition.display.outputOrdering : definition.display.inputOrdering,
      nibID => ConnectionNib.NibConnection(nibID) == connectionSide.nib,
    )
    {columns: isSink ? -1 : graphWidth - 2, rows: nibIndex + 1}
  | NodeConnection(nodeID) =>
    let nodePosition = Belt.Map.String.getExn(nodeLayouts, nodeID).position
    let nibIndex = NodeGetNibIndex.f(
      Belt.Map.String.getExn(nodes, nodeID),
      definitions,
      connectionSide.nib,
      isSink,
    )
    {columns: nodePosition.columns, rows: nodePosition.rows + nibIndex}
  }
