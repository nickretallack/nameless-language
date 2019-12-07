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
    | _ => raise(Not_found)
    };
  externalFunction(inputs, outputID);
};
