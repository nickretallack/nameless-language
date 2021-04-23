let f = (connectionNode: ConnectionNode.t): Js.Json.t => {
  open Json.Encode
  object_(
    switch connectionNode {
    | GraphConnection => list{("type", string("graph"))}
    | NodeConnection(nodeID) => list{("type", string("node")), ("nodeID", string(nodeID))}
    },
  )
}
