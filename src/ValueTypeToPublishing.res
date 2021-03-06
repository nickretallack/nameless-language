let f = (valueType: ValueType.t, dependencies: PublishingDependencyMap.t): PublishingValueType.t =>
  switch valueType {
  | DefinedValueType(definitionID) =>
    PublishingDefinedValueType(Belt.Map.String.getExn(dependencies, definitionID).contentID)
  | PrimitiveValueType(primitiveValueType) => PublishingPrimitiveValueType(primitiveValueType)
  | AnyType => PublishingAnyType
  }
