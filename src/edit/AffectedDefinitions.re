open Definition;
let findUses =
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
