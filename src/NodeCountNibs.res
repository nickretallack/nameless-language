let f = (node: Node.t, definitions: DefinitionMap.t): int =>
  switch node.kind {
  | ListNode(length) => length
  | DefinedNode({kind, definitionID}) =>
    let nodeDefinition = Belt.Map.String.getExn(definitions, definitionID)
    let nodeDisplay = DefinedNodeGetDisplayNibs.f(nodeDefinition, kind, "en")
    Belt.Array.length(nodeDisplay.inputs) + Belt.Array.length(nodeDisplay.outputs)
  }
