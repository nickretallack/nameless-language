let f = (json: Js.Json.t): ConnectionNib.t => {
  open Json.Decode
  open ConnectionNib
  switch field("type", string, json) {
  | "value" => ValueConnection
  | "nib" => NibConnection(field("nibID", string, json))
  | "positional" => PositionalConnection(field("index", int, json))
  | type_ => raise(Exception.JsonDecodeInvalidTypeName(type_, "ConnectionNib"))
  }
}
