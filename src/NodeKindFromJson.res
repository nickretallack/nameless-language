let f = (json: Js.Json.t): NodeKind.t => {
  open Json.Decode
  open NodeKind
  switch field("type", string, json) {
  | "list" => ListNode(field("length", int, json))
  | "defined" => DefinedNode(DefinedNodeFromJson.f(json))
  | type_ => raise(Exception.JsonDecodeInvalidTypeName(type_, "NodeKind"))
  }
}
