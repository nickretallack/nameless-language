type t = {
  definitionID: DefinitionID.t,
  sourceValues: Belt.Map.t<ConnectionSide.t, Value.t, ConnectionSideComparable.C.identity>,
  nodeScopeIDs: Belt.Map.String.t<ScopeID.t>,
  callingScope: option<ParentScope.t>,
  scopeType: ScopeType.t,
}
