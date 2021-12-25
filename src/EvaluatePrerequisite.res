let f = (
  inputs: Belt.Map.String.t<option<Value.t>>,
  outputID: string,
): ExternalEvaluationResult.t => {
  Js.log2("start", inputs)
  EvaluateWithAllValuesResult.f(inputs, values => {
    Js.log2("inside", values)
    if outputID == "value" {
      Belt.Map.String.getExn(values, "value")
    } else {
      Value.Prerequisite
    }
  })
}
