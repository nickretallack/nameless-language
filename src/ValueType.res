type t =
  | PrimitiveValueType(PrimitiveValueType.t)
  | DefinedValueType(DefinitionID.t)
  | AnyType
