let f = (json: Js.Json.t): AppState.t =>
  Json.Decode.(
    AppState.{
      definitions:
        field("definitions", MapFromJson.f(DefinitionFromJson.f), json),
      languageName: field("languageName", string, json),
      execution: None,
      error: NoAppError,
    }
  );
