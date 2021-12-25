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
  | "concatenateText" => EvaluateConcatenateText.f
  | "addKeyboardEventListener" => EvaluateAddKeyboardEventListener.f
  | "scheduleRecurringEvent" => EvaluateScheduleRecurringEvent.f
  | "scheduleAnimationFrame" => EvaluateScheduleAnimationFrame.f
  | "prerequisite" => EvaluatePrerequisite.f
  | "htmlCreateElement" => EvaluateHtmlCreateElement.f
  | "htmlSetCssProperty" => EvaluateHtmlSetCssProperty.f
  | "log" => EvaluateLog.f(execution, definitions, languageName)
  | _ => raise(Exception.UnknownExternal(name))
  }
  externalFunction(inputs, outputID)
}
