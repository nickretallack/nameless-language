let f = (
  definitionID: DefinitionID.t,
  definition: Definition.t,
  dependencies: PublishingDependencyMap.t,
): string =>
  ImplementationToCanonicalString.f(
    definitionID,
    definition.implementation,
    definition.display,
    Belt.Map.String.set(
      dependencies,
      definitionID,
      {
        kind: Recursion,
        inputOrdering: definition.display.inputOrdering,
        outputOrdering: definition.display.outputOrdering,
      },
    ),
  )
