let f = (json: Js.Json.t): Node.t => {
  open Json.Decode
  open Node
  {
    scope: field("scope", NodeScopeFromJson.f, json),
    kind: field("kind", NodeKindFromJson.f, json),
  }
}
