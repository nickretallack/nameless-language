type t = {
  definitionID: DefinitionID.t,
  sourceValues: SourceValues.t,
  nodeScopeIDs: Belt.Map.String.t<ScopeID.t>,
  callingScope: option<ParentScope.t>,
  scopeType: ScopeType.t,
}
