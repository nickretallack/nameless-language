let f = (json: Js.Json.t): Node.t =>
  Json.Decode.(
    Node.{
      scope: field("scope", NodeScopeFromJson.f, json),
      kind: field("kind", NodeKindFromJson.f, json),
    }
  );
