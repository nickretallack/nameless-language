let f = (definitionID: DefinitionID.t, parentScope: option<ParentScope.t>): Scope.t => {
  definitionID: definitionID,
  parentScope: parentScope,
  sourceValues: Belt.Map.make(~id=module(ConnectionSideComparable.C)),
  nodeScopeIDs: Belt.Map.String.empty,
}
