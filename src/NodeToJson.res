let f = (node: Node.t): Js.Json.t => {
  open Json.Encode
  object_(list{("scope", NodeScopeToJson.f(node.scope)), ("kind", NodeKindToJson.f(node.kind))})
}
