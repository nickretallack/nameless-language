let f = (nodeKind: NodeKind.t): Js.Json.t => {
  open Json.Encode
  switch nodeKind {
  | ListNode(length) => object_(list{("type", string("list")), ("length", int(length))})
  | DefinedNode(definedNode) => DefinedNodeToJson.f(definedNode)
  }
}
