let rec f = (nodeIDs: Belt.Set.String.t, nodes: NodeMap.t) =>
  Belt.Set.String.reduce(nodeIDs, nodeIDs, (acc, nodeID) =>
    if NodeIsFunctionDefinition.f(Belt.Map.String.getExn(nodes, nodeID)) {
      let children = Belt.Set.String.fromArray(
        Belt.Map.String.keysToArray(
          Belt.Map.String.keep(nodes, (_nodeID, node) =>
            switch node.scope {
            | NodeScope(scopeNodeID) => scopeNodeID == nodeID
            | _ => false
            }
          ),
        ),
      )
      Belt.Set.String.union(acc, f(children, nodes))
    } else {
      acc
    }
  )
