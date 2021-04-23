let f = (json: Js.Json.t): DefinedNode.t => {
  open Json.Decode
  open DefinedNode
  {
    definitionID: field("definitionID", string, json),
    kind: field("kind", DefinedNodeKindFromJson.f, json),
  }
}
