let f = (
  interface: Interface.t,
  dependencies: PublishingDependencyMap.t,
  display: DefinitionDisplay.t,
): PublishingInterface.t => {
  inputs: RecordTypeToPublishing.f(interface.input, dependencies, display.inputOrdering),
  outputs: RecordTypeToPublishing.f(interface.output, dependencies, display.outputOrdering),
}
