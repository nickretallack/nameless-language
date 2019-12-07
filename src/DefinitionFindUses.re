let f =
    (definitionID: DefinitionID.t, definitions: DefinitionMap.t)
    : DefinitionMap.t =>
  Belt.Map.String.keep(definitions, (_, otherDefinition) =>
    switch (otherDefinition.implementation) {
    | GraphImplementation({nodes}) =>
      Belt.Map.String.some(nodes, (_, node) =>
        switch (node.kind) {
        | DefinedNode({definitionID: usingDefinitionID}) =>
          usingDefinitionID == definitionID
        | _ => false
        }
      )
    | _ => false
    }
  );
