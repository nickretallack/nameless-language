let f =
    (definitionID: DefinitionID.t, parentScope: option(ParentScope.t))
    : Scope.t => {
  definitionID,
  parentScope,
  sourceValues: Belt.Map.make(~id=(module ConnectionSideComparable.C)),
  nodeScopeIDs: Belt.Map.String.empty,
};
