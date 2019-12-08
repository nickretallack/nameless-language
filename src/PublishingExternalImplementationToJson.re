let f = (publishingExternal: PublishingExternalImplementation.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("interface")),
      (
        "inputs",
        PublishingValueTypeListToJson.f(publishingExternal.interface.inputs),
      ),
      (
        "outputs",
        PublishingValueTypeListToJson.f(publishingExternal.interface.outputs),
      ),
    ])
  );
