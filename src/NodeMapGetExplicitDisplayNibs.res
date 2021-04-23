let f = (nodes: NodeMap.t, definitions: DefinitionMap.t, languageName: LanguageName.t): list<
  ExplicitDisplayNib.t,
> =>
  Belt.List.reduce(Belt.Map.String.toList(nodes), list{}, (acc, (nodeID, node)) => {
    let {
      NodeDisplayNibs.inputs: inputs,
      outputs,
      internalInputs,
      internalOutputs,
    } = NodeGetDisplayNibs.f(node, definitions, languageName)
    let connectionNode = ConnectionNode.NodeConnection(nodeID)
    Belt.List.concatMany([
      acc,
      DisplayNibsToExplicit.f(inputs, connectionNode, false),
      DisplayNibsToExplicit.f(outputs, connectionNode, true),
      DisplayNibsToExplicit.f(internalInputs, connectionNode, true),
      DisplayNibsToExplicit.f(internalOutputs, connectionNode, false),
    ])
  })
