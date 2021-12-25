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
and t =
  | PrimitiveValue(PrimitiveValue.t)
  | DefinedValue(definedValueRecord)
  | LazyValue(lazyValue)
  | InlineFunction(NodeInScope.t)
  | Prerequisite
  | Reference(ReferenceID.t)
  | HTMLElement(Webapi.Dom.Element.t)
  | ScheduledEventIdentifier(ScheduledEventIdentifier.t)
