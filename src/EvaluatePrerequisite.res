let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  outputID: string,
): ExternalEvaluationResult.t => {
  EvaluateWithAllValuesResult.f(inputs, values => {
    if outputID == "value" {
      Belt.Map.String.getExn(values, "value")
    } else {
      Value.Prerequisite
    }
  })
}
