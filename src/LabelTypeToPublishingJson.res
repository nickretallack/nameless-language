let f = (
  id: DefinitionID.t,
  valueType: ValueType.t,
  dependencies: PublishingDependencyMap.t,
): Js.Json.t => PublishingLabelTypeToJson.f(id, ValueTypeToPublishing.f(valueType, dependencies))
