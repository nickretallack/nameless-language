@react.component
let make = (
  ~execution: option<Execution.t>,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
) => {
  switch execution {
  | Some(execution) =>
    let (rootScopeID, rootScope) = Belt.Option.getExn(
      Belt.Map.String.findFirstBy(execution.scopes, (_scopeID, scope) =>
        Belt.Option.isNone(scope.callingContext)
      ),
    )
    let scopeByParent = Belt.Map.String.reduce(execution.scopes, Belt.Map.String.empty, (
      mapping,
      scopeID,
      scope,
    ) =>
      switch scope.callingContext {
      | Some(callingContext) =>
        Belt.Map.String.update(mapping, callingContext.callingScopeID, scopes =>
          switch scopes {
          | Some(scopes) => Some(Belt.List.add(scopes, (scopeID, scope)))
          | None => Some(Belt.List.fromArray([(scopeID, scope)]))
          }
        )
      | None => mapping
      }
    )
    <ExecutionScopeTree
      execution scope=rootScope scopeID=rootScopeID languageName definitions scopeByParent
    />
  | None => React.null
  }
}
