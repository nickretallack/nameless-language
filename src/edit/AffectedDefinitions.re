open Definition;
let findDefinitionUses =
    (definitionID: definitionID, definitions: definitions): definitions =>
  Belt.Map.String.keep(definitions, (_, otherDefinition) =>
    switch (otherDefinition.implementation) {
    | GraphImplementation({nodes}) =>
      Belt.Map.String.some(nodes, (_, node) =>
        switch (node.kind) {
        | DefinedNode({definitionID: usingDefinitionID}) =>
          usingDefinitionID == definitionID
        | _ => false
        }
      )
    | _ => false
    }
  );

let findConnectedDefinitions =
    (
      definitionID: definitionID,
      nibID: nibID,
      isInput: bool,
      definitions: definitions,
    )
    : definitions =>
  Belt.Map.String.keep(definitions, (_, otherDefinition) =>
    switch (otherDefinition.implementation) {
    | GraphImplementation({nodes, connections}) =>
      let checkConnectionSide =
          (connectionSide: connectionSide, nodeID: nodeID) =>
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
            isInput ?
              switch (kind) {
              | FunctionCallNode
              | FunctionPointerCallNode
              | ConstructorNode => checkSink()
              | FunctionDefinitionNode => checkSource()
              | _ => false
              } :
              (
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

let removeNibFromInterface =
    (interface: interface, nibID: nibID, isInput: bool): interface =>
  isInput ?
    {
      ...interface,
      inputTypes:
        Belt.Map.String.keep(interface.inputTypes, (typeNibID, _) =>
          typeNibID != nibID
        ),
    } :
    {
      ...interface,
      outputTypes:
        Belt.Map.String.keep(interface.outputTypes, (typeNibID, _) =>
          typeNibID != nibID
        ),
    };

/* let findUses

   let removeNodesByID =
       (definition: definition, nodeIDs: list(nodeID)): definition => {
     ...definition,
     implementation:
       switch (definition.implementation) {
       | GraphImplementation(graphImplementation) =>
         let nodeIDs =
           ExpandDeletion.getAffectedNodes(nodeIDs, graphImplementation.nodes);
         GraphImplementation({
           ...graphImplementation,
           nodes:
             Belt.Map.String.removeMany(
               graphImplementation.nodes,
               Belt.Set.String.toArray(nodeIDs),
             ),
           connections:
             Belt.Map.keep(graphImplementation.connections, (sink, source) =>
               !(
                 ExpandDeletion.connectionSideInvolvesNodeIDs(sink, nodeIDs)
                 || ExpandDeletion.connectionSideInvolvesNodeIDs(source, nodeIDs)
               )
             ),
         });
       | _ => raise(Not_found)
       },
   };

   let getNodeIDsByDefinition =
       (definition: definition, definitionID: definitionID) =>
     switch (definition.implementation) {
     | GraphImplementation({nodes}) =>
       Belt.Map.String.some(nodes, (_, node) =>
         switch (node.kind) {
         | DefinedNode({definitionID: usingDefinitionID}) =>
           usingDefinitionID == definitionID
         | _ => false
         }
       )
     | _ => false
     }; */
