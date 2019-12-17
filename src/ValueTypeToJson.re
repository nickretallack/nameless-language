let f = (valueType: ValueType.t): Js.Json.t =>
  Json.Encode.(
    object_(
      switch (valueType) {
      | PrimitiveValueType(primitiveValueType) => [
          ("type", string("primitive")),
          ("primitiveType", PrimitiveValueTypeToJson.f(primitiveValueType)),
        ]
      | DefinedValueType(definitionID) => [
          ("type", string("defined")),
          ("definitionID", string(definitionID)),
        ]
      | AnyType => [("type", string("any"))]
      },
    )
  );
