let f = (connectionNode: ConnectionNode.t): Js.Json.t =>
  Json.Encode.(
    object_(
      switch (connectionNode) {
      | GraphConnection => [("type", string("graph"))]
      | NodeConnection(nodeID) => [
          ("type", string("node")),
          ("nodeID", string(nodeID)),
        ]
      },
    )
  );
