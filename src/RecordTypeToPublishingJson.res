let f = (
  typedFields: RecordType.t,
  dependencies: PublishingDependencyMap.t,
  fieldOrdering: array<NibID.t>,
): Js.Json.t =>
  PublishingRecordTypeToJson.f(RecordTypeToPublishing.f(typedFields, dependencies, fieldOrdering))
