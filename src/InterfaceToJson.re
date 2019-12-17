let f = (interface: Interface.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("interface")),
      ("inputs", TypedFieldsToJson.f(interface.input)),
      ("outputs", TypedFieldsToJson.f(interface.output)),
    ])
  );
