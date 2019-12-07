let f =
    (
      inputs: Belt.Map.String.t(option(Value.t)),
      operation: Belt.Map.String.t(Value.t) => Value.t,
    )
    : ExternalEvaluationResult.t => {
  let (values, needed) =
    Belt.Map.String.reduce(
      inputs, (Belt.Map.String.empty, []), ((values, needed), key, value) =>
      switch (value) {
      | None => (values, Belt.List.add(needed, key))
      | Some(value) => (Belt.Map.String.set(values, key, value), needed)
      }
    );
  if (Belt.List.length(needed) != 0) {
    EvaluationRequired([Belt.List.headExn(needed)]);
  } else {
    EvaluationResult(operation(values));
  };
};
