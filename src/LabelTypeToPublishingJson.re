let f =
    (
      id: DefinitionID.t,
      maybeValueType: option(ValueType.t),
      dependencies: PublishingDependencyMap.t,
    )
    : Js.Json.t =>
  PublishingLabelTypeToJson.f(
    id,
    switch (maybeValueType) {
    | None => None
    | Some(valueType) =>
      Some(ValueTypeToPublishing.f(valueType, dependencies))
    },
  );
