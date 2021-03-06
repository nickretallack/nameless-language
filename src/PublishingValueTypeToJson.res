let f = (valueType: PublishingValueType.t) => {
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
  }
}
