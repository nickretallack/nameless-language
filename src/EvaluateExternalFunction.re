let f =
    (
      name: string,
      outputID: string,
      inputs: Belt.Map.String.t(option(Value.t)),
    )
    : ExternalEvaluationResult.t => {
  let externalFunction =
    switch (name) {
    | "branch" => EvaluateConditionalBranch.f
    | "+" => EvaluateNumericOperator.f((+.))
    | "-" => EvaluateNumericOperator.f((-.))
    | "*" => EvaluateNumericOperator.f(( *. ))
    | "/" => EvaluateNumericOperator.f((/.))
    | "<" => EvaluateNumericComparison.f((<))
    | ">" => EvaluateNumericComparison.f((>))
    | "<=" => EvaluateNumericComparison.f((<=))
    | ">=" => EvaluateNumericComparison.f((>=))
    | "==" => EvaluateNumericComparison.f((==))
    | _ => raise(Exception.UnknownExternal(name))
    };
  externalFunction(inputs, outputID);
};
