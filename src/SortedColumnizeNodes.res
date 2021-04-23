let f = (
  nodes: NodeMap.t,
  connections: ConnectionMap.t,
  definitions: DefinitionMap.t,
  display: DefinitionDisplay.t,
): list<list<NodeWithID.t>> => {
  let columns = ColumnizeNodes.f(nodes, connections)
  let nodeToColumnIndex = Belt.List.reduceWithIndex(columns, Belt.Map.String.empty, (
    acc,
    column,
    index,
  ) => Belt.List.reduce(column, acc, (acc, node) => Belt.Map.String.set(acc, node.id, index)))
  Belt.List.map(columns, column =>
    ListSortBy.f(column, nodeWithID => {
      let relevantConnections = Belt.Map.keep(connections, (_sink, source) =>
        switch source.node {
        | NodeConnection(nodeID) => nodeID == nodeWithID.id
        | _ => false
        }
      )

      Belt.Map.reduce(relevantConnections, (max_int, max_int, max_int), (acc, sink, _source) => {
        let newRanking = RankConnection.f(sink, columns, nodeToColumnIndex, definitions, display)
        compare(acc, newRanking) < 0 ? acc : newRanking
      })
    })
  )
}
