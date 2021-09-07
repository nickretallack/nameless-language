type rec t =
  | PublishingPrimitiveValueType(PrimitiveValueType.t)
  | PublishingDefinedValueType(ContentID.t)
  | PublishingAnyType
  | PublishingSequencerType
  | PublishingReferenceType(t)
