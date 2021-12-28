let f = (
  definitionID: DefinitionID.t,
  callingContext: option<CallingContext.t>,
  scopeType: ScopeType.t,
): Scope.t => {
  definitionID: definitionID,
  callingContext: callingContext,
  scopeType: scopeType,
  sourceValues: Belt.Map.make(~id=module(ConnectionSideComparable.C)),
  nodeScopeIDs: Belt.Map.String.empty,
}
