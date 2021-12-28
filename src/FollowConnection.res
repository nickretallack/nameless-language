let f = (
  sink: ConnectionSide.t,
  scopeID: ScopeID.t,
  scopes: ScopeMap.t,
  definitions: DefinitionMap.t,
): option<ConnectionSide.t> => {
  let definitionID = ScopeGetGraphDefinitionID.f(scopes, scopeID)
  let definition = Belt.Map.String.getExn(definitions, definitionID)
  let graphImplementation = DefinitionAssertGraph.f(definition)
  Belt.Map.get(graphImplementation.connections, sink)
}
