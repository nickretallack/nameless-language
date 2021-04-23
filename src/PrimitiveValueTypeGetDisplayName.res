let f = (primitiveValueType: PrimitiveValueType.t): string =>
  switch primitiveValueType {
  | IntegerType => "Integer"
  | NumberType => "Number"
  | TextType => "Text"
  }
