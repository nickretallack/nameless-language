open Definition;

type externalEvaluationResult =
  | EvaluationResult(Definition.value)
  | EvaluationRequired(list(string));

let evaluateInput =
    (inputs: Belt.Map.String.t(option(value)), inputID: string)
    : externalEvaluationResult => {
  switch (Belt.Map.String.getExn(inputs, inputID)) {
  | None => EvaluationRequired([inputID])
  | Some(value) => EvaluationResult(value)
  };
};

let conditionalBranch =
    (inputs: Belt.Map.String.t(option(value)), outputID: string)
    : externalEvaluationResult => {
  if (outputID != "result") {
    raise(Not_found);
  };
  switch (Belt.Map.String.getExn(inputs, "if")) {
  | None => EvaluationRequired(["if"])
  | Some(DefinedValue({definitionID: "yes"})) =>
    evaluateInput(inputs, "then")
  | Some(DefinedValue({definitionID: "no"})) =>
    evaluateInput(inputs, "else")
  | _ => raise(Not_found)
  };
};

let withAllValues =
    (
      inputs: Belt.Map.String.t(option(value)),
      operation: Belt.Map.String.t(value) => value,
    )
    : externalEvaluationResult => {
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

let addition = (inputs: Belt.Map.String.t(option(value)), outputID: string) => {
  if (outputID != "result") {
    raise(Not_found);
  };
  withAllValues(inputs, values =>
    PrimitiveValue(
      NumberValue(
        Evaluate.getNumber(Belt.Map.String.getExn(values, "left"))
        +. Evaluate.getNumber(Belt.Map.String.getExn(values, "right")),
      ),
    )
  );
};

let evaluateExternal =
    (
      name: string,
      outputID: string,
      inputs: Belt.Map.String.t(option(value)),
    )
    : externalEvaluationResult => {
  let externalFunction =
    switch (name) {
    | "branch" => conditionalBranch
    | "+" => addition
    | _ => raise(Not_found)
    };
  externalFunction(inputs, outputID);
};
