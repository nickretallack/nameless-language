let f =
    (definition: Definition.t, connectionNib: ConnectionNib.t, isSink: bool)
    : int =>
  switch (connectionNib) {
  | ValueConnection => 0
  | PositionalConnection(_) => raise(Not_found)
  | NibConnection(_) =>
    let nibs = DefinitionGetKeywordDisplayNibs.f(definition, "en", !isSink);
    ListFindByIndexExn.f(nibs, ({nib}) => nib == connectionNib);
  };
