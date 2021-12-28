let f = (
  sink: ConnectionSide.t,
  scopeID: ScopeID.t,
  scopes: ScopeMap.t,
  definitions: DefinitionMap.t,
) => {
  let definitionID = ScopeGetGraphDefinitionID.f(scopes, scopeID)
  let definition = Belt.Map.String.getExn(definitions, definitionID)
  let graphImplementation = DefinitionAssertGraph.f(definition)
  Belt.Map.getExn(graphImplementation.connections, sink)
}
