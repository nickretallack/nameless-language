let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  inputsNeeded: array<NibID.t>,
  operation: Belt.Map.String.t<Value.t> => ExternalEvaluationResult.t,
): ExternalEvaluationResult.t => {
  let (values, needed) = Belt.Array.reduce(inputsNeeded, (Belt.Map.String.empty, list{}), (
    (values, needed),
    nibID,
  ) =>
    switch Belt.Map.String.getExn(inputs, nibID) {
    | None
    | Some(LazyValue(_)) => (values, Belt.List.add(needed, nibID))
    | Some(value) => (Belt.Map.String.set(values, nibID, value), needed)
    }
  )
  if Belt.List.length(needed) != 0 {
    EvaluationRequired(list{Belt.List.headExn(needed)})
  } else {
    operation(values)
  }
}
