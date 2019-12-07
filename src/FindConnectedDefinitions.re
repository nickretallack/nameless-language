let f =
    (
      definitionID: DefinitionID.t,
      nibID: NibID.t,
      isInput: bool,
      definitions: DefinitionMap.t,
    )
    : DefinitionMap.t =>
  Belt.Map.String.keep(definitions, (_, otherDefinition) =>
    switch (otherDefinition.implementation) {
    | GraphImplementation({nodes, connections}) =>
      let checkConnectionSide =
          (connectionSide: ConnectionSide.t, nodeID: NodeID.t) =>
        switch (connectionSide) {
        | {node: NodeConnection(aNodeID), nib: NibConnection(aNibID)} =>
          aNodeID == nodeID && aNibID == nibID
        | _ => false
        };

      Belt.Map.String.some(nodes, (nodeID, node) =>
        switch (node.kind) {
        | DefinedNode({definitionID: usingDefinitionID, kind}) =>
          let checkSink = (_: unit) =>
            Belt.Map.some(connections, (sink, _source) =>
              checkConnectionSide(sink, nodeID)
            );
          let checkSource = (_: unit) =>
            Belt.Map.some(connections, (_sink, source) =>
              checkConnectionSide(source, nodeID)
            );

          usingDefinitionID == definitionID
          && (
            isInput
              ? switch (kind) {
                | FunctionCallNode
                | FunctionPointerCallNode
                | ConstructorNode => checkSink()
                | FunctionDefinitionNode => checkSource()
                | _ => false
                }
              : (
                switch (kind) {
                | FunctionCallNode
                | FunctionPointerCallNode => checkSource()
                | FunctionDefinitionNode
                | AccessorNode => checkSink()
                | _ => false
                }
              )
          );
        | _ => false
        }
      );
    | _ => false
    }
  );
