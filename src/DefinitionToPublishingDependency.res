let f = (
  definitionID: DefinitionID.t,
  definition: Definition.t,
  dependencies: PublishingDependencyMap.t,
): PublishingDependency.t => {
  let canonicalString = ImplementationToCanonicalString.f(
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
  let contentID = ReScriptHash.Sha256.make(canonicalString)
  {
    kind: Final({
      contentID: contentID,
      canonicalString: canonicalString,
    }),
    inputOrdering: definition.display.inputOrdering,
    outputOrdering: definition.display.outputOrdering,
  }
}
