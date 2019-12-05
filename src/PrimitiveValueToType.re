let f = (primitiveValue: PrimitiveValue.t): PrimitiveValueType.t =>
  switch (primitiveValue) {
  | IntegerValue(_) => IntegerType
  | NumberValue(_) => NumberType
  | TextValue(_) => TextType
  };
