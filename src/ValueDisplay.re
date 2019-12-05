let f = (value: Value.t): string =>
  switch (value) {
  | PrimitiveValue(primitiveValue) => PrimitiveValueDisplay.f(primitiveValue)
  | DefinedValue(_) => "TODO: defined value"
  | LazyValue(_) => "(not computed yet)"
  };
