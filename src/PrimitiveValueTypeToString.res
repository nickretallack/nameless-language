let f = (primitiveValueType: PrimitiveValueType.t): string =>
  switch primitiveValueType {
  | IntegerType => "integer"
  | NumberType => "number"
  | TextType => "text"
  }
