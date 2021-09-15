let f = (
  definitionID: DefinitionID.t,
  definition: Definition.t,
  dependencies: PublishingDependencyMap.t,
): PublishingDependency.t => {
  let canonicalString = DefinitionToCanonicalString.f(definitionID, definition, dependencies)
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
