type rec definedValue =
  | RecordValue(Belt.Map.String.t<t>)
  | SymbolValue
  | LabeledValue(t)
  | FunctionPointerValue
and definedValueRecord = {
  definitionID: DefinitionID.t,
  value: definedValue,
}
and lazyValue = {
  scopeID: ScopeID.t,
  explicitConnectionSide: ExplicitConnectionSide.t,
}
and inlineFunction = {
  // TODO: unify with InlineScope
  scopeID: ScopeID.t,
  nodeID: NodeID.t,
}
and t =
  | PrimitiveValue(PrimitiveValue.t)
  | DefinedValue(definedValueRecord)
  | LazyValue(lazyValue)
  | InlineFunction(inlineFunction)
  | Prerequisite
