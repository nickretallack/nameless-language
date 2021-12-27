let rec f = (
  scope: Scope.t,
  source: ConnectionSide.t,
  execution: Execution.t,
  definitions: DefinitionMap.t,
): option<Value.t> => {
  let value = Belt.Map.get(scope.sourceValues, source)
  switch value {
  | Some(LazyValue(lazyValue)) =>
    switch LazyValueResolve.f(lazyValue, definitions, execution.scopes) {
    | Some(value) => Some(value)
    | None => value
    }
  | None =>
    // Special case for graph inputs and nodes that have already been entered
    switch source.node {
    | GraphConnection =>
      switch scope.callingScope {
      | Some({nodeID, scopeID}) =>
        // Check the calling scope.
        let callingScope = Belt.Map.String.getExn(execution.scopes, scopeID)
        let definition = Belt.Map.String.getExn(definitions, callingScope.definitionID)
        let graphImplementation = DefinitionAssertGraph.f(definition)
        let sink: ConnectionSide.t = {
          node: NodeConnection(nodeID),
          nib: source.nib,
        }
        let newSource = Belt.Map.getExn(graphImplementation.connections, sink)
        f(callingScope, newSource, execution, definitions)
      | None => raise(Exception.ShouldntHappen("Reached the top of the call stack"))
      }
    | NodeConnection(nodeID) =>
      switch Belt.Map.String.get(scope.nodeScopeIDs, nodeID) {
      | Some(scopeID) =>
        // Node was already entered.  Check the child scope.
        let childScope = Belt.Map.String.getExn(execution.scopes, scopeID)
        let definition = Belt.Map.String.getExn(definitions, childScope.definitionID)
        let graphImplementation = DefinitionAssertGraph.f(definition)
        let sink: ConnectionSide.t = {
          node: GraphConnection,
          nib: source.nib,
        }
        let newSource = Belt.Map.getExn(graphImplementation.connections, sink)
        f(childScope, newSource, execution, definitions)
      | None =>
        switch scope.scopeType {
        | InlineScope({scopeID}) =>
          // Check the outer scope.
          let inlineScope = Belt.Map.String.getExn(execution.scopes, scopeID)
          f(inlineScope, source, execution, definitions)
        | GraphScope => value
        }
      }
    }
  | value => value
  }
}
