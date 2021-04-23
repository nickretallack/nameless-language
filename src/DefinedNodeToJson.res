let f = (definedNode: DefinedNode.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("type", string("defined")),
    ("definitionID", string(definedNode.definitionID)),
    ("kind", string(DefinedNodeKindToString.f(definedNode.kind))),
  })
}
