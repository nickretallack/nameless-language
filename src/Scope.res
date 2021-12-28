type t = {
  definitionID: DefinitionID.t,
  sourceValues: SourceValues.t,
  nodeScopeIDs: Belt.Map.String.t<ScopeID.t>,
  callingContext: option<CallingContext.t>,
  scopeType: ScopeType.t,
}
