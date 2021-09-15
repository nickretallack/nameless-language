let rec f = (
  valueType: ValueType.t,
  dependencies: PublishingDependencyMap.t,
): PublishingValueType.t =>
  switch valueType {
  | DefinedValueType(definitionID) =>
    PublishingDefinedValueType(
      PublishingDependencyKindToContentID.f(
        Belt.Map.String.getExn(dependencies, definitionID).kind,
      ),
    )
  | PrimitiveValueType(primitiveValueType) => PublishingPrimitiveValueType(primitiveValueType)
  | AnyType => PublishingAnyType
  | SequencerType => PublishingSequencerType
  | ReferenceType(referenceType) => PublishingReferenceType(f(referenceType, dependencies))
  | HTMLElementType => PublishingHTMLElementType
  }
