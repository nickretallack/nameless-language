let f = (
  operation: (float, float) => float,
  inputs: Belt.Map.String.t<option<Value.t>>,
  outputID: string,
): ExternalEvaluationResult.t => {
  if outputID != "result" {
    raise(Not_found)
  }
  EvaluateWithAllValues.f(inputs, values => PrimitiveValue(
    NumberValue(
      operation(
        ValueToFloat.f(Belt.Map.String.getExn(values, "left")),
        ValueToFloat.f(Belt.Map.String.getExn(values, "right")),
      ),
    ),
  ))
}
