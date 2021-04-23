let f = (node: NodeWithID.t, connections: ConnectionMap.t, scopes: NodeScopeSet.t): bool =>
  Belt.Set.has(scopes, node.node.scope) &&
  !Belt.Map.some(connections, (sink: ConnectionSide.t, source: ConnectionSide.t) =>
    switch source.node {
    | GraphConnection => false
    | NodeConnection(connectionNodeID) =>
      node.id != connectionNodeID
        ? false
        : switch node.node.kind {
          | DefinedNode({kind: FunctionDefinitionNode}) =>
            switch source.nib {
            | ValueConnection =>
              switch sink.node {
              | NodeConnection(_) => true
              | GraphConnection => false
              }
            | NibConnection(_) => false
            | _ => raise(Not_found)
            }
          | _ =>
            switch sink.node {
            | NodeConnection(_) => true
            | GraphConnection => false
            }
          }
    }
  )
