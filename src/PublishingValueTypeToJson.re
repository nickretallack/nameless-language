let f = (valueType: PublishingValueType.t) =>
  Json.Encode.(
    switch (valueType) {
    | PublishingPrimitiveValueType(primitiveValueType) =>
      object_([
        ("type", string("primitive")),
        (
          "primitiveType",
          string(PrimitiveValueTypeToString.f(primitiveValueType)),
        ),
      ])
    | PublishingDefinedValueType(contentID) =>
      object_([
        ("type", string("defined")),
        ("contentID", string(contentID)),
      ])
    | PublishingAnyType => object_([("type", string("any"))])
    }
  );
