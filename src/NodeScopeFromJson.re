let f = (json: Js.Json.t): NodeScope.t =>
  Json.Decode.(
    NodeScope.(
      switch (field("type", string, json)) {
      | "graph" => GraphScope
      | "node" => NodeScope(field("nodeID", string, json))
      | _ => raise(Exception.JsonDecodeInvalidTypeName)
      }
    )
  );
