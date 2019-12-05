type t =
  | PrimitiveValueTypeForPublishing(PrimitiveValueType.t)
  | DefinedValueTypeForPublishing(ContentID.t)
  | AnyTypeForPublishing;
