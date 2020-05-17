let f = (id: DefinitionID.t, valueType: PublishingValueType.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("label")),
      ("id", string(id)),
      ("wrapped", PublishingValueTypeToJson.f(valueType)),
    ])
  );
