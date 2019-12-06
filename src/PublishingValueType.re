type t =
  | PublishingPrimitiveValueType(PrimitiveValueType.t)
  | PublishingDefinedValueType(ContentID.t)
  | PublishingAnyType;
