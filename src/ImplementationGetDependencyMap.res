let rec worker = (
  definitionID: DefinitionID.t,
  definitions: DefinitionMap.t,
  dependencyMap: DependencyMap.t,
) => {
  let definition = Belt.Map.String.getExn(definitions, definitionID)
  let dependencies = ImplementationGetDirectDependencies.f(definition.implementation)
  let dependencyMap = Belt.Map.String.set(dependencyMap, definitionID, dependencies)
  Belt.Set.String.reduce(dependencies, dependencyMap, (dependencyMap, dependencyDefinitionID) => {
    if !Belt.Map.String.has(dependencyMap, dependencyDefinitionID) {
      worker(dependencyDefinitionID, definitions, dependencyMap)
    } else {
      dependencyMap
    }
  })
}

let f = (definitionID: DefinitionID.t, definitions: DefinitionMap.t): DependencyMap.t =>
  worker(definitionID, definitions, Belt.Map.String.empty)
