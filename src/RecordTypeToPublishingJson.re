let f =
    (
      typedFields: RecordType.t,
      dependencies: PublishingDependencyMap.t,
      fieldOrdering: list(NibID.t),
    )
    : Js.Json.t =>
  PublishingRecordTypeToJson.f(
    RecordTypeToPublishing.f(typedFields, dependencies, fieldOrdering),
  );
