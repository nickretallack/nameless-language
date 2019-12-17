let f = (json: Js.Json.t): Definition.t =>
  DefinitionMake.f(
    "en",
    ~implementation=Implementation.LabeledTypeImplementation(None),
    (),
  );
