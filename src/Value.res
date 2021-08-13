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
  connectionSide: ConnectionSide.t,
}
and inlineFunction = {
  scopeID: ScopeID.t,
  nodeID: NodeID.t,
}
and t =
  | PrimitiveValue(PrimitiveValue.t)
  | DefinedValue(definedValueRecord)
  | LazyValue(lazyValue)
  | InlineFunction(inlineFunction)
  | Prerequisite
