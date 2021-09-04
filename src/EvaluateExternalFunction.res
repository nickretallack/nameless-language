let f = (
  execution: Execution.t,
  definitions: DefinitionMap.t,
  languageName: LanguageName.t,
  name: string,
  outputID: string,
  inputs: Belt.Map.String.t<option<Value.t>>,
): ExternalEvaluationResult.t => {
  let externalFunction = switch name {
  | "branch" => EvaluateConditionalBranch.f
  | "+" => EvaluateNumericOperator.f(\"+.")
  | "-" => EvaluateNumericOperator.f(\"-.")
  | "*" => EvaluateNumericOperator.f(\"*.")
  | "/" => EvaluateNumericOperator.f(\"/.")
  | "<" => EvaluateNumericComparison.f(\"<")
  | ">" => EvaluateNumericComparison.f(\">")
  | "<=" => EvaluateNumericComparison.f(\"<=")
  | ">=" => EvaluateNumericComparison.f(\">=")
  | "=" => EvaluateEquals.f
  | "addKeyboardEventListener" => EvaluateAddKeyboardEventListener.f
  | "log" => EvaluateLog.f(execution, definitions, languageName)
  | _ => raise(Exception.UnknownExternal(name))
  }
  externalFunction(inputs, outputID)
}
