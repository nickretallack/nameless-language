let f = (primitiveValue: PrimitiveValue.t) =>
  switch (primitiveValue) {
  | IntegerValue(value) => string_of_int(value)
  | NumberValue(value) => FloatToString.f(value)
  | TextValue(value) => value
  };
