let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  operation: Belt.Map.String.t<Value.t> => Value.t,
): ExternalEvaluationResult.t =>
  EvaluateWithValues.f(inputs, Belt.Map.String.keysToArray(inputs), values => EvaluationResult(
    operation(values),
  ))
