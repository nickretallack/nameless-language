let f = (interface: Interface.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("type", string("interface")),
    ("input", TypedFieldsToJson.f(interface.input)),
    ("output", TypedFieldsToJson.f(interface.output)),
  })
}
