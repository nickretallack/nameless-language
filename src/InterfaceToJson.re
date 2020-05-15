let f = (interface: Interface.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("interface")),
      ("input", TypedFieldsToJson.f(interface.input)),
      ("output", TypedFieldsToJson.f(interface.output)),
    ])
  );
