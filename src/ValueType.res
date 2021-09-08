type rec t =
  | PrimitiveValueType(PrimitiveValueType.t)
  | DefinedValueType(DefinitionID.t)
  | SequencerType
  | AnyType
  | ReferenceType(t)
  | HTMLElementType
