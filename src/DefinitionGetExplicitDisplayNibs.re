let f =
    (definition: Definition.t, definitions: DefinitionMap.t)
    : list(ExplicitDisplayNib.t) => {
  switch (definition.implementation) {
  | GraphImplementation(graphImplementation) =>
    Belt.List.concatMany([|
      DisplayNibsToExplicit.f(
        DefinitionGetKeywordDisplayNibOutputs.f(definition, "en"),
        GraphConnection,
        false,
      ),
      DisplayNibsToExplicit.f(
        DefinitionGetKeywordDisplayNibInputs.f(definition, "en"),
        GraphConnection,
        true,
      ),
      NodeMapGetExplicitDisplayNibs.f(graphImplementation.nodes, definitions),
    |])
  | _ => raise(Not_found)
  };
};
