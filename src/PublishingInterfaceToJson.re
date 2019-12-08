let f = (interface: PublishingInterface.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("interface")),
      ("inputs", PublishingValueTypeListToJson.f(interface.inputs)),
      ("outputs", PublishingValueTypeListToJson.f(interface.outputs)),
    ])
  );
