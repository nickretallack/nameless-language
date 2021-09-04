let f = (
  execution: Execution.t,
  definitions: DefinitionMap.t,
  languageName: LanguageName.t,
  inputs: Belt.Map.String.t<option<Value.t>>,
  _outputID: string,
): ExternalEvaluationResult.t => {
  switch Belt.Map.String.getExn(inputs, "value") {
  | None => EvaluationRequired(list{"value"})
  | Some(value) =>
    Js.log(ValueDisplay.f(value, execution, definitions, languageName))
    EvaluationResult(value)
  }
}
