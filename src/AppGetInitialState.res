let f = (): AppState.t => {
  open Dom.Storage
  let appState = switch getItem(AppStateName.v, localStorage) {
  | None =>
    open AppState
    {
      definitions: DefinitionExamples.v,
      error: NoAppError,
      execution: None,
      languageName: "en",
      autoSave: false,
    }
  | Some(jsonString) =>
    switch Json.parse(jsonString) {
    | Some(json) => AppStateFromPersistenceJson.f(json)
    | None => raise(Exception.JsonDecodeFailed)
    }
  }

  // overwrite builtins
  {
    ...appState,
    definitions: Belt.Map.String.mergeMany(appState.definitions, DefinitionExamples.builtins),
  }
}
