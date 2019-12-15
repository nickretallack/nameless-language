let f =
    (
      definition: Definition.t,
      definitions: DefinitionMap.t,
      languageName: LanguageName.t,
    )
    : list(ExplicitDisplayNib.t) => {
  switch (definition.implementation) {
  | GraphImplementation(graphImplementation) =>
    Belt.List.concatMany([|
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
      NodeMapGetExplicitDisplayNibs.f(graphImplementation.nodes, definitions),
    |])
  | _ => raise(Not_found)
  };
};
