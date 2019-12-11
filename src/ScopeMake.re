let f = (definitionID: DefinitionID.t): Scope.t => {
  definitionID,
  sourceValues: Belt.Map.make(~id=(module ConnectionSideComparable.C)),
  nodeScopeIDs: Belt.Map.String.empty,
};
