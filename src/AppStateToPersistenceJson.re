let f = (state: AppState.t): Js.Json.t =>
  Json.Encode.(
    object_([
      (
        "definitions",
        StringMapToJson.f(state.definitions, DefinitionToJson.f),
      ),
      ("languageName", string(state.languageName)),
    ])
  );
