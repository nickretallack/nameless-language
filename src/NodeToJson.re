let f = (node: Node.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("scope", NodeScopeToJson.f(node.scope)),
      ("kind", NodeKindToJson.f(node.kind)),
    ])
  );
