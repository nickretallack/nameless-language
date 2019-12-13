let f =
    (definitionID: DefinitionID.t, parentScopeID: option(ScopeID.t)): Scope.t => {
  definitionID,
  parentScopeID,
  sourceValues: Belt.Map.make(~id=(module ConnectionSideComparable.C)),
  nodeScopeIDs: Belt.Map.String.empty,
};
