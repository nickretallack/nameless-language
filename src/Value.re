type definedValue = {
  definitionID: DefinitionID.t,
  values: Belt.Map.String.t(t),
}
and evaluationAction =
  | Evaluating
  | Returning(t)
and stackFrame = {
  scopeID: ScopeID.t,
  explicitConnectionSide: ExplicitConnectionSide.t,
  action: evaluationAction,
}
and t =
  | PrimitiveValue(PrimitiveValue.t)
  | DefinedValue(definedValue)
  | LazyValue(stackFrame);
