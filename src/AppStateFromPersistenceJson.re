let f = (json: Js.Json.t): AppState.t =>
  Json.Decode.(
    AppState.{
      definitions:
        field(
          "definitions",
          StringMapFromJson.f(DefinitionFromJson.f),
          json,
        ),
      languageName: field("languageName", string, json),
      execution: None,
      error: NoAppError,
      autoSave: true,
    }
  );
