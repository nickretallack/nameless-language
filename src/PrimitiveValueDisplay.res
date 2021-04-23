let f = (primitiveValue: PrimitiveValue.t): string =>
  switch primitiveValue {
  | TextValue(value) => "\"" ++ (value ++ "\"")
  | _ => PrimitiveValueToString.f(primitiveValue)
  }
