let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  outputID: string,
): ExternalEvaluationResult.t => {
  if outputID != "result" {
    raise(Not_found)
  }
  EvaluateWithAllValuesResult.f(inputs, values => PrimitiveValue(
    TextValue(
      `${ValueCoerceText.f(Belt.Map.String.getExn(values, "left"))}${ValueCoerceText.f(
          Belt.Map.String.getExn(values, "right"),
        )}`,
    ),
  ))
}
