let f = (value: Value.t): float =>
  switch (value) {
  | PrimitiveValue(NumberValue(number)) => number
  | _ => raise(Not_found)
  };
