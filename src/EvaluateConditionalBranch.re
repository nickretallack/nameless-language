let f =
    (inputs: Belt.Map.String.t(option(Value.t)), outputID: string)
    : ExternalEvaluationResult.t => {
  if (outputID != "result") {
    raise(Not_found);
  };
  switch (Belt.Map.String.getExn(inputs, "if")) {
  | None => EvaluationRequired(["if"])
  | Some(DefinedValue({definitionID: "yes"})) =>
    EvaluateInput.f(inputs, "then")
  | Some(DefinedValue({definitionID: "no"})) =>
    EvaluateInput.f(inputs, "else")
  | _ => raise(Not_found)
  };
};
