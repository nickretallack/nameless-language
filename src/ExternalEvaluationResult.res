type t =
  | EvaluationResult(Value.t)
  | EvaluationRequired(list<string>)
