type t = {
  definitionID: DefinitionID.t,
  sourceValues: SourceValues.t,
  nodeScopeIDs: Belt.Map.String.t<ScopeID.t>,
  callingScope: option<NodeInScope.t>,
  scopeType: ScopeType.t,
}
