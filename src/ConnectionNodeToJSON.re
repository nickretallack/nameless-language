let f = (connectionNode: ConnectionNode.t) =>
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
