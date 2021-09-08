let f = (value: Value.t): string =>
  switch value {
  | PrimitiveValue(primitiveValue) => PrimitiveValueToString.f(primitiveValue)
  | _ => raise(Exception.TypeError("Expected a string"))
  }
