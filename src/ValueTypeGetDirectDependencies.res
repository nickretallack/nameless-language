let rec f = (valueType: ValueType.t): Belt.Set.String.t =>
  switch valueType {
  | DefinedValueType(definitionID) => Belt.Set.String.fromArray([definitionID])
  | ReferenceType(valueType) => f(valueType)
  | PrimitiveValueType(_)
  | SequencerType
  | AnyType
  | ScheduledEventIdentifierType
  | HTMLElementType => Belt.Set.String.empty
  }
