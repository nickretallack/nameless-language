let f = (nodeScope: NodeScope.t): Js.Json.t => {
  open Json.Encode
  object_(
    switch nodeScope {
    | GraphScope => list{("type", string("graph"))}
    | NodeScope(nodeID) => list{("type", string("node")), ("nodeID", string(nodeID))}
    },
  )
}
