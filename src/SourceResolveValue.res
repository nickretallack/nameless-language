let f = (
  scope: Scope.t,
  connectionSide: ConnectionSide.t,
  execution: Execution.t,
  definitions: DefinitionMap.t,
) => {
  let value = Belt.Map.get(scope.sourceValues, connectionSide)
  switch value {
  | Some(LazyValue(lazyValue)) =>
    switch LazyValueResolve.f(lazyValue, definitions, execution.scopes) {
    | Some(value) => Some(value)
    | None => value
    }
  | value => value
  }
}
