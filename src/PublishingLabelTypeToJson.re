let f =
    (id: DefinitionID.t, valueType: option(PublishingValueType.t)): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("label")),
      ("id", string(id)),
      (
        "wrapped",
        switch (valueType) {
        | Some(valueType) => PublishingValueTypeToJson.f(valueType)
        | None => null
        },
      ),
    ])
  );
