let f = (graphImplementation: GraphImplementation.t, nodeIDs: Belt.Set.String.t): (
  Belt.Map.String.t<Node.t>,
  Belt.Map.String.t<Node.t>,
  NodeScopeSet.t,
) => {
  let (innerNodes, outerNodes) = Belt.Map.String.partition(graphImplementation.nodes, (
    nodeID,
    _node,
  ) => Belt.Set.String.has(nodeIDs, nodeID))

  let scopes = Belt.Map.String.reduce(innerNodes, NodeScopeSet.fromArray([]), (
    scopes,
    _nodeID,
    node,
  ) => Belt.Set.add(scopes, node.scope))
  (innerNodes, outerNodes, scopes)
}
