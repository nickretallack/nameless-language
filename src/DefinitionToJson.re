let f = (definition: Definition.t): Js.Json.t =>
  Json.Encode.(
    object_([
      ("implementation", ImplementationToJson.f(definition.implementation)),
      ("documentation", DocumentationToJson.f(definition.documentation)),
      ("display", DefinitionDisplayToJson.f(definition.display)),
    ])
  );
