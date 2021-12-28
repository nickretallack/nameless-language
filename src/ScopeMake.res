let f = (
  definitionID: DefinitionID.t,
  callingScope: option<CallingScope.t>,
  scopeType: ScopeType.t,
): Scope.t => {
  definitionID: definitionID,
  callingScope: callingScope,
  scopeType: scopeType,
  sourceValues: Belt.Map.make(~id=module(ConnectionSideComparable.C)),
  nodeScopeIDs: Belt.Map.String.empty,
}
