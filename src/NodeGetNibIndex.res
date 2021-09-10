let f = (
  node: Node.t,
  definitions: DefinitionMap.t,
  connectionNib: ConnectionNib.t,
  isSink: bool,
): int =>
  switch node.kind {
  | DefinedNode({definitionID, kind: FunctionDefinitionNode}) =>
    FunctionDefinitionGetNibIndex.f(
      Belt.Map.String.getExn(definitions, definitionID),
      connectionNib,
      isSink,
    ) + 1
  | _ =>
    let {NodeDisplayNibs.inputs: inputs, outputs} = NodeGetDisplayNibs.f(node, definitions, "en")
    let nibs = isSink ? inputs : outputs
    let nibIndex = ArrayFindIndexByExn.f(nibs, ({nib}) => nib == connectionNib)
    (isSink ? nibIndex : nibIndex + Belt.Array.length(inputs)) + 1
  }
