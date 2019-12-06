let f =
    (
      node: Node.t,
      definitions: DefinitionMap.t,
      connectionNib: ConnectionNib.t,
    )
    : int => {
  switch (node.kind) {
  | DefinedNode({definitionID, kind: FunctionDefinitionNode}) =>
    FunctionDefinitionGetNibIndex.f(
      Belt.Map.String.getExn(definitions, definitionID),
      connectionNib,
      false,
    )
  | _ =>
    let NodeDisplayNibs.{outputs} =
      NodeGetDisplayNibs.f(node, definitions, "en");
    ListFindByIndexExn.f(outputs, ({nib}) => nib == connectionNib);
  };
};
