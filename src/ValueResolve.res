let rec resolveSource = (
  scope: Scope.t,
  source: ConnectionSide.t,
  scopes: ScopeMap.t,
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
    | GraphConnection => resolveInCaller(scope, source.nib, scopes, definitions)
    | NodeConnection(nodeID) =>
      switch Belt.Map.String.get(scope.nodeScopeIDs, nodeID) {
      | Some(calledScopeID) =>
        // Node was already entered.  Check the child scope.
        let calledScope = Belt.Map.String.getExn(scopes, calledScopeID)
        let nodeConnection: ConnectionNode.t = switch calledScope.scopeType {
        | GraphScope => GraphConnection
        | InlineScope({nodeID}) => NodeConnection(nodeID)
        }
        let sink: ConnectionSide.t = {
          node: nodeConnection,
          nib: source.nib,
        }
        let newSource = FollowConnection.f(sink, calledScopeID, scopes, definitions)
        resolveSource(calledScope, newSource, scopes, definitions)
      | None =>
        switch scope.scopeType {
        | InlineScope({parentScopeID, nodeID: inlineDefinitionNodeID}) =>
          if nodeID == inlineDefinitionNodeID {
            // Ensure it's an interior connection, not the implementation nib.
            switch source.nib {
            | NibConnection(_) => resolveInCaller(scope, source.nib, scopes, definitions)
            | _ => value
            }
          } else {
            // Check the outer scope.
            let inlineScope = Belt.Map.String.getExn(scopes, parentScopeID)
            resolveSource(inlineScope, source, scopes, definitions)
          }

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
    FollowConnection.f(
      lazyValue.explicitConnectionSide.connectionSide,
      lazyValue.scopeID,
      scopes,
      definitions,
    )
  }
  resolveSource(scope, source, scopes, definitions)
}
and resolveInCaller = (
  scope: Scope.t,
  nib: ConnectionNib.t,
  scopes: Belt.Map.String.t<Scope.t>,
  definitions: DefinitionMap.t,
) =>
  switch scope.callingContext {
  | Some({nodeID, callingScopeID}) =>
    // Check the calling scope.
    let callingScope = Belt.Map.String.getExn(scopes, callingScopeID)
    let sink: ConnectionSide.t = {
      node: NodeConnection(nodeID),
      nib: nib,
    }
    let newSource = FollowConnection.f(sink, callingScopeID, scopes, definitions)
    resolveSource(callingScope, newSource, scopes, definitions)
  | None => raise(Exception.ShouldntHappen("Reached the top of the call stack"))
  }
