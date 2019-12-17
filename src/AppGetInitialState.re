let f = (): AppState.t =>
  Dom.Storage.(
    switch (getItem(AppStateName.v, localStorage)) {
    | None =>
      AppState.{
        definitions: DefinitionExamples.v,
        error: NoAppError,
        execution: None,
        languageName: "en",
      }
    | Some(jsonString) =>
      switch (Json.parse(jsonString)) {
      | Some(json) => AppStateFromPersistenceJson.f(json)
      | None => raise(Not_found)
      }
    }
  );
