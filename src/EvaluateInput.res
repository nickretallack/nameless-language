let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  inputID: NibID.t,
): ExternalEvaluationResult.t =>
  switch Belt.Map.String.getExn(inputs, inputID) {
  | None => EvaluationRequired(list{inputID})
  | Some(value) => EvaluationResult(value)
  }
