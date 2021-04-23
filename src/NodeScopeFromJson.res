let f = (json: Js.Json.t): NodeScope.t => {
  open Json.Decode
  open NodeScope
  switch field("type", string, json) {
  | "graph" => GraphScope
  | "node" => NodeScope(field("nodeID", string, json))
  | type_ => raise(Exception.JsonDecodeInvalidTypeName(type_, "NodeScope"))
  }
}
