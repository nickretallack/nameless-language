let f = (primitiveValue: PrimitiveValue.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("type", string("constant")),
      ("value", PrimitiveValueToJson.f(primitiveValue)),
    ])
  );
