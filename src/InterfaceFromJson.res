let f = (json: Js.Json.t): Interface.t => {
  open Json.Decode
  open Interface
  {
    input: field("input", TypedFieldsFromJson.f, json),
    output: field("output", TypedFieldsFromJson.f, json),
  }
}
