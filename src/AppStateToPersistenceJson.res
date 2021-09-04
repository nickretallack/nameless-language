let f = (state: AppState.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    (
      "definitions",
      StringMapToJson.f(
        Belt.Map.String.keep(state.definitions, (definitionID, _) =>
          !Belt.Map.String.has(DefinitionExamples.builtinsMap, definitionID)
        ),
        DefinitionToJson.f,
      ),
    ),
    ("languageName", string(state.languageName)),
  })
}
