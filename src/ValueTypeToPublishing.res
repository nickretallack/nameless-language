let rec f = (
  valueType: ValueType.t,
  dependencies: PublishingDependencyMap.t,
): PublishingValueType.t =>
  switch valueType {
  | DefinedValueType(definitionID) =>
    PublishingDefinedValueType(
      switch Belt.Map.String.getExn(dependencies, definitionID).kind {
      | Final({contentID}) => contentID
      | Recursion => "self"
      | MutualRecursion(_definition) => "mutual" // TODO: base it on the definition
      },
    )
  | PrimitiveValueType(primitiveValueType) => PublishingPrimitiveValueType(primitiveValueType)
  | AnyType => PublishingAnyType
  | SequencerType => PublishingSequencerType
  | ReferenceType(referenceType) => PublishingReferenceType(f(referenceType, dependencies))
  | HTMLElementType => PublishingHTMLElementType
  }
