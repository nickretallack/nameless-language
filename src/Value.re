type definedValue = {
  definitionID: DefinitionID.t,
  values: Belt.Map.String.t(t),
}
and lazyValue = {
  scopeID: ScopeID.t,
  explicitConnectionSide: ExplicitConnectionSide.t,
}
and t =
  | PrimitiveValue(PrimitiveValue.t)
  | DefinedValue(definedValue)
  | LazyValue(lazyValue);
