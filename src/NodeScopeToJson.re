let f = (nodeScope: NodeScope.t): Js.Json.t =>
  Json.Encode.(
    object_(
      switch (nodeScope) {
      | GraphScope => [("type", string("graph"))]
      | NodeScope(nodeID) => [
          ("type", string("node")),
          ("nodeID", string(nodeID)),
        ]
      },
    )
  );
