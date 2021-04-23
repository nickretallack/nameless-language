let f = (
  externalImplementation: ExternalImplementation.t,
  dependencies: PublishingDependencyMap.t,
  display: DefinitionDisplay.t,
): PublishingExternalImplementation.t => {
  name: externalImplementation.name,
  interface: InterfaceToPublishing.f(externalImplementation.interface, dependencies, display),
}
