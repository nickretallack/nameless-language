let rec f = (scopes: Belt.Map.String.t<Scope.t>, scopeID: ScopeID.t): DefinitionID.t => {
  let scope = Belt.Map.String.getExn(scopes, scopeID)
  switch scope.scopeType {
  | GraphScope => scope.definitionID
  | InlineScope({parentScopeID}) => f(scopes, parentScopeID)
  }
}
