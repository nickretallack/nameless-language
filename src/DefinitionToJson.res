let f = (definition: Definition.t): Js.Json.t => {
  open Json.Encode
  object_(list{
    ("implementation", ImplementationToJson.f(definition.implementation)),
    ("documentation", DocumentationToJson.f(definition.documentation)),
    ("display", DefinitionDisplayToJson.f(definition.display)),
  })
}
