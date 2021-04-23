let f = (
  externalImplementation: ExternalImplementation.t,
  dependencies: PublishingDependencyMap.t,
  display: DefinitionDisplay.t,
): Js.Json.t =>
  PublishingExternalImplementationToJson.f(
    ExternalImplementationToPublishing.f(externalImplementation, dependencies, display),
  )
