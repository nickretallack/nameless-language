let f = (json: Js.Json.t): PrimitiveValueType.t =>
  PrimitiveValueTypeFromString.f(Json.Decode.string(json))
