let f = (json: Js.Json.t): ConnectionNode.t => {
  open Json.Decode
  open ConnectionNode
  switch field("type", string, json) {
  | "graph" => GraphConnection
  | "node" => NodeConnection(field("nodeID", string, json))
  | type_ => raise(Exception.JsonDecodeInvalidTypeName(type_, "ConnectionNode"))
  }
}
