let f =
    (
      comparison: (float, float) => bool,
      inputs: Belt.Map.String.t(option(Value.t)),
      outputID: string,
    ) => {
  if (outputID != "result") {
    raise(Not_found);
  };
  EvaluateWithAllValues.f(inputs, values =>
    DefinedValue({
      definitionID:
        comparison(
          ValueToFloat.f(Belt.Map.String.getExn(values, "left")),
          ValueToFloat.f(Belt.Map.String.getExn(values, "right")),
        )
          ? "yes" : "no",
      value: Value.LabeledValue(None),
    })
  );
};
