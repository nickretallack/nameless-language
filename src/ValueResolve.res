let rec resolveSource = (
  scope: Scope.t,
  source: ConnectionSide.t,
  scopes: Belt.Map.String.t<Nameless.Scope.t>,
  definitions: DefinitionMap.t,
): option<Value.t> => {
  let value = Belt.Map.get(scope.sourceValues, source)
  switch value {
  | Some(LazyValue(lazyValue)) =>
    switch resolveLazyValue(lazyValue, definitions, scopes) {
    | Some(value) => Some(value)
    | None => value
    }
  | None =>
    // Special case for graph inputs and nodes that have already been entered
    switch source.node {
    | GraphConnection =>
      switch scope.callingScope {
      | Some({nodeID, callingScopeID}) =>
        // Check the calling scope.
        let callingScope = Belt.Map.String.getExn(scopes, callingScopeID)
        let definition = Belt.Map.String.getExn(definitions, callingScope.definitionID)
        let graphImplementation = DefinitionAssertGraph.f(definition)
        let sink: ConnectionSide.t = {
          node: NodeConnection(nodeID),
          nib: source.nib,
        }
        let newSource = Belt.Map.getExn(graphImplementation.connections, sink)
        resolveSource(callingScope, newSource, scopes, definitions)
      | None => raise(Exception.ShouldntHappen("Reached the top of the call stack"))
      }
    | NodeConnection(nodeID) =>
      switch Belt.Map.String.get(scope.nodeScopeIDs, nodeID) {
      | Some(scopeID) =>
        // Node was already entered.  Check the child scope.
        let childScope = Belt.Map.String.getExn(scopes, scopeID)
        let definition = Belt.Map.String.getExn(definitions, childScope.definitionID)
        let graphImplementation = DefinitionAssertGraph.f(definition)
        let sink: ConnectionSide.t = {
          node: GraphConnection,
          nib: source.nib,
        }
        let newSource = Belt.Map.getExn(graphImplementation.connections, sink)
        resolveSource(childScope, newSource, scopes, definitions)
      | None =>
        switch scope.scopeType {
        | InlineScope({scopeID}) =>
          // Check the outer scope.
          let inlineScope = Belt.Map.String.getExn(scopes, scopeID)
          resolveSource(inlineScope, source, scopes, definitions)
        | GraphScope => value
        }
      }
    }
  | value => value
  }
}
and resolveLazyValue = (
  lazyValue: Value.lazyValue,
  definitions: DefinitionMap.t,
  scopes: Belt.Map.String.t<Nameless.Scope.t>,
): option<Value.t> => {
  let scope = Belt.Map.String.getExn(scopes, lazyValue.scopeID)
  let source = if lazyValue.explicitConnectionSide.isSource {
    lazyValue.explicitConnectionSide.connectionSide
  } else {
    // Follow connection to source
    let definition = Belt.Map.String.getExn(definitions, scope.definitionID)
    let graphImplementation = DefinitionAssertGraph.f(definition)
    Belt.Map.getExn(
      graphImplementation.connections,
      lazyValue.explicitConnectionSide.connectionSide,
    )
  }
  resolveSource(scope, source, scopes, definitions)
}
