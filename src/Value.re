type definedValue = {
  definitionID: DefinitionID.t,
  values: Belt.Map.String.t(t),
}
and lazyValue = {
  scopeID: ScopeID.t,
  connectionSide: ConnectionSide.t,
}
and t =
  | PrimitiveValue(PrimitiveValue.t)
  | DefinedValue(definedValue)
  | LazyValue(lazyValue);
