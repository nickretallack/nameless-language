let rec f = (valueType: PublishingValueType.t) => {
  open Json.Encode
  switch valueType {
  | PublishingPrimitiveValueType(primitiveValueType) =>
    object_(list{
      ("type", string("primitive")),
      ("primitiveType", string(PrimitiveValueTypeToString.f(primitiveValueType))),
    })
  | PublishingDefinedValueType(contentID) =>
    object_(list{("type", string("defined")), ("contentID", string(contentID))})
  | PublishingAnyType => object_(list{("type", string("any"))})
  | PublishingSequencerType => object_(list{("type", string("sequencer"))})
  | PublishingReferenceType(valueType) =>
    object_(list{("type", string("reference")), ("referenceType", f(valueType))})
  | PublishingHTMLElementType => object_(list{("type", string("html element"))})
  | PublishingScheduledEventIdentifierType =>
    object_(list{("type", string("scheduled event identifier"))})
  }
}
