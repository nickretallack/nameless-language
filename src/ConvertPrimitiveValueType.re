let f = (primitiveValue: PrimitiveValue.t, typeName: string): PrimitiveValue.t =>
  switch (typeName) {
  | "text" => TextValue(PrimitiveValueToString.f(primitiveValue))
  | "number" =>
    NumberValue(
      try(float_of_string(PrimitiveValueToString.f(primitiveValue))) {
      | Failure(_) => 0.0
      },
    )
  | "integer" =>
    IntegerValue(
      switch (primitiveValue) {
      | NumberValue(value) => FloatToInt.f(value)
      | _ => int_of_string(PrimitiveValueToString.f(primitiveValue))
      },
    )
  | _ =>
    raise(
      Exception.JsonDecodeInvalidTypeName(
        typeName,
        "ConvertPrimitiveValueType",
      ),
    )
  };
