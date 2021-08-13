let f = (
  name: string,
  outputID: string,
  inputs: Belt.Map.String.t<option<Value.t>>,
  webView,
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
  | "==" => EvaluateNumericComparison.f(\"=")
  | "addEventListener" => EvaluateAddEventListener.f(webView)
  | _ => raise(Exception.UnknownExternal(name))
  }
  externalFunction(inputs, outputID)
}
