let f = (
  definition: Definition.t,
  definitions: DefinitionMap.t,
  languageName: LanguageName.t,
): array<ExplicitDisplayNib.t> =>
  switch definition.implementation {
  | GraphImplementation(graphImplementation) =>
    Belt.Array.concatMany([
      DisplayNibsToExplicit.f(
        DefinitionGetKeywordDisplayNibOutputs.f(definition, languageName),
        GraphConnection,
        false,
      ),
      DisplayNibsToExplicit.f(
        DefinitionGetKeywordDisplayNibInputs.f(definition, languageName),
        GraphConnection,
        true,
      ),
      NodeMapGetExplicitDisplayNibs.f(graphImplementation.nodes, definitions, languageName),
    ])
  | _ => raise(Exception.ShouldntHappen("DefinitionGetExplicitDisplayNibs on non-graph"))
  }
