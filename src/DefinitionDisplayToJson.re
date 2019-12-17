let f = (display: DefinitionDisplay.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("inputOrdering", list(string, display.inputOrdering)),
      ("outputOrdering", list(string, display.outputOrdering)),
    ])
  );
