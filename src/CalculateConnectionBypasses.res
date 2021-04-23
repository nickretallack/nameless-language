let f = (
  nodeLayouts: Belt.Map.String.t<NodeLayout.t>,
  connections: ConnectionMap.t,
  nodes: NodeMap.t,
  definition: Definition.t,
  definitions: DefinitionMap.t,
  graphWidth: int,
): Belt.Map.t<ConnectionSide.t, list<int>, ConnectionSideComparable.C.identity> =>
  Belt.Map.mapWithKey(connections, (sink, source) => {
    let startPosition = GetNibPosition.f(
      sink,
      true,
      nodeLayouts,
      nodes,
      definition,
      graphWidth,
      definitions,
    )
    let endPosition = GetNibPosition.f(
      source,
      false,
      nodeLayouts,
      nodes,
      definition,
      graphWidth,
      definitions,
    )
    let length = endPosition.columns - startPosition.columns - 1
    let parentScopes = Belt.Set.String.fromArray(
      Belt.List.toArray(
        switch sink.node {
        | GraphConnection => list{}
        | NodeConnection(nodeID) => GetParentScopes.f(nodeID, nodes)
        },
      ),
    )
    // starts at the source.
    // ignores all parent scopes of the sink
    // collision detects with all other nodes at each iteration
    // can use color index as a slight vertical offset later
    Belt.List.reverse(
      fst(
        Belt.List.reduce(Belt.List.makeBy(length, index => index), (list{}, endPosition.rows), (
          (results, rows),
          index,
        ) => {
          let columns = endPosition.columns - index - 1
          let position = {
            open NodePosition
            {columns: columns, rows: rows}
          }
          let collisions = Belt.Map.String.keep(nodeLayouts, (nodeID, layout) =>
            !Belt.Set.String.has(parentScopes, nodeID) && CollisionDetect.f(layout, position)
          )
          let rows = if Belt.Map.String.isEmpty(collisions) {
            rows
          } else {
            let collisions = Belt.List.fromArray(Belt.Map.String.valuesToArray(collisions))
            let outermostCollision = Belt.List.reduce(List.tl(collisions), List.hd(collisions), (
              acc,
              layout,
            ) => layout.depth > acc.depth ? layout : acc)
            let bottom = outermostCollision.position.rows + outermostCollision.size.rows
            if abs(outermostCollision.position.rows - rows) < abs(bottom - rows) {
              outermostCollision.position.rows - 1
            } else {
              bottom
            }
          }
          (list{rows, ...results}, rows)
        }),
      ),
    )
  })
