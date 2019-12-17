let f = (state: AppState.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("definitions", MapToJson.f(state.definitions, DefinitionToJson.f)),
      ("languageName", string(state.languageName)),
    ])
  );
