let rec f = (
  valueType: ValueType.t,
  definitions: DefinitionMap.t,
  language: LanguageName.t,
): string =>
  switch valueType {
  | PrimitiveValueType(primitiveValueType) => PrimitiveValueTypeGetDisplayName.f(primitiveValueType)
  | DefinedValueType(definitionID) =>
    DefinedTypeGetDisplayName.f(Belt.Map.String.getExn(definitions, definitionID), language)
  | AnyType => "Any"
  | SequencerType => "Sequencer"
  | ReferenceType(referenceType) => `Reference to ${f(referenceType, definitions, language)}`
  }
