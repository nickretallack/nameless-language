let f = (nodeKind: NodeKind.t): Js.Json.t => {
  open Json.Encode
  switch nodeKind {
  | ReferenceNode => object_(list{("type", string("reference"))})
  | ListNode(length) => object_(list{("type", string("list")), ("length", int(length))})
  | DefinedNode(definedNode) => DefinedNodeToJson.f(definedNode)
  }
}
