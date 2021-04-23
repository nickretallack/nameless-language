let f = (primitiveValue: PrimitiveValue.t): Js.Json.t => {
  open Json.Encode
  object_(list{("type", string("constant")), ("value", PrimitiveValueToJson.f(primitiveValue))})
}
