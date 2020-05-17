let f =
    (definition: Definition.t, connectionNib: ConnectionNib.t, isSink: bool)
    : int =>
  switch (connectionNib) {
  | ValueConnection => 0
  | PositionalConnection(_) =>
    raise(
      Exception.TODO("PositionalConnection in FunctionDefinitionGetNibIndex"),
    )
  | NibConnection(_) =>
    let nibs = DefinitionGetKeywordDisplayNibs.f(definition, "en", !isSink);
    ListFindByIndexExn.f(nibs, ({nib}) => nib == connectionNib);
  };
