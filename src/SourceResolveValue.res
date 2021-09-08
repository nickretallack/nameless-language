let rec f = (
  scope: Scope.t,
  connectionSide: ConnectionSide.t,
  execution: Execution.t,
  definitions: DefinitionMap.t,
): option<Value.t> => {
  let value = Belt.Map.get(scope.sourceValues, connectionSide)
  switch value {
  | Some(LazyValue(lazyValue)) =>
    switch LazyValueResolve.f(lazyValue, definitions, execution.scopes) {
    | Some(value) => Some(value)
    | None => value
    }
  | None =>
    switch scope.scopeType {
    | InlineScope({scopeID}) =>
      let inlineScope = Belt.Map.String.getExn(execution.scopes, scopeID)
      f(inlineScope, connectionSide, execution, definitions)
    | GraphScope => value
    }
  | value => value
  }
}
