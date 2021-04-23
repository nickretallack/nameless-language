let f = (state: AppState.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("definitions", StringMapToJson.f(state.definitions, DefinitionToJson.f)),
    ("languageName", string(state.languageName)),
  })
}
