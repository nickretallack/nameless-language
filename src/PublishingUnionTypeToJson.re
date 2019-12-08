let f = (fields: list(PublishingValueType.t)): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("union")),
      ("types", PublishingValueTypeListToJson.f(fields)),
    ])
  );
