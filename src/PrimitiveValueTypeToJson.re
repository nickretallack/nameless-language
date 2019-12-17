let f = (primitiveValueType: PrimitiveValueType.t): Js.Json.t =>
  Json.Encode.string(PrimitiveValueTypeToString.f(primitiveValueType))
