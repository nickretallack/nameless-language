open Definition;

type externalEvaluationResult =
  | EvaluationResult(Definition.value)
  | EvaluationRequired(int);

let evaluateIndex =
    (inputs: list(option(value)), index: int): externalEvaluationResult => {
  switch (Belt.List.getExn(inputs, index)) {
  | None => EvaluationRequired(index)
  | Some(value) => EvaluationResult(value)
  };
};

let conditionalBranch =
    (inputs: list(option(value)), outputIndex: int)
    : externalEvaluationResult => {
  if (outputIndex != 0) {
    raise(Not_found);
  };
  switch (Belt.List.getExn(inputs, 0)) {
  | None => EvaluationRequired(0)
  | Some(DefinedValue({definitionID: "yes"})) => evaluateIndex(inputs, 1)
  | Some(DefinedValue({definitionID: "no"})) => evaluateIndex(inputs, 2)
  | _ => raise(Not_found)
  };
};

let evaluateExternal =
    (name: string, outputIndex: int, inputs: list(option(value)))
    : externalEvaluationResult => {
  let externalFunction =
    switch (name) {
    | "branch" => conditionalBranch
    | _ => raise(Not_found)
    };
  externalFunction(inputs, outputIndex);
};
