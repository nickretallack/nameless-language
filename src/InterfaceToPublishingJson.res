let f = (
  interface: Interface.t,
  dependencies: PublishingDependencyMap.t,
  display: DefinitionDisplay.t,
): Js.Json.t =>
  PublishingInterfaceToJson.f(InterfaceToPublishing.f(interface, dependencies, display))
