[%%debugger.chrome];
open Definition;
open! AppActions;
open Helpers;
open Evaluate;

type appState = {
  execution: option(execution),
  definitions,
  definitionID,
  error: appError,
};

let reducer = (action: appAction, state: appState) =>
  switch (action) {
  | ChangeRoute(url) =>
    ReasonReact.Update({...state, definitionID: url.hash, error: NoAppError})
  | CreateDefinition(definition) =>
    let definitionID = randomID();
    ReasonReact.UpdateWithSideEffects(
      {
        ...state,
        definitions:
          Belt.Map.String.set(state.definitions, definitionID, definition),
      },
      _ => ReasonReact.Router.push("#" ++ definitionID),
    );
  | DefinitionAction({definitionID, action}) =>
    let definition = Belt.Map.String.getExn(state.definitions, definitionID);

    let updateDefinition = definition =>
      ReasonReact.Update({
        ...state,
        definitions:
          Belt.Map.String.set(state.definitions, definitionID, definition),
        error: NoAppError,
      });

    switch (action) {
    | CreateConnection({source, sink}) =>
      switch (definition.implementation) {
      | GraphImplementation(graphImplementation) =>
        updateDefinition({
          ...definition,
          implementation:
            GraphImplementation({
              ...graphImplementation,
              connections:
                Belt.Map.set(graphImplementation.connections, sink, source),
            }),
        })
      | _ => ReasonReact.NoUpdate
      }

    | AddNode({node, explicitConnectionSide, connectionNib}) =>
      let nodeID = randomID();
      let nodeConnectionSide: connectionSide = {
        node: NodeConnection(nodeID),
        nib: connectionNib,
      };
      let (source, sink) =
        explicitConnectionSide.isSource ?
          (explicitConnectionSide.connectionSide, nodeConnectionSide) :
          (nodeConnectionSide, explicitConnectionSide.connectionSide);
      switch (definition.implementation) {
      | GraphImplementation(graphImplementation) =>
        updateDefinition({
          ...definition,
          implementation:
            GraphImplementation({
              ...graphImplementation,
              nodes:
                Belt.Map.String.set(graphImplementation.nodes, nodeID, node),
              connections:
                Belt.Map.set(graphImplementation.connections, sink, source),
            }),
        })
      | _ => ReasonReact.NoUpdate
      };
    | ChangeConstantValue(implementation) =>
      switch (definition.implementation) {
      | ConstantImplementation(_) =>
        updateDefinition({
          ...definition,
          implementation: ConstantImplementation(implementation),
        })
      | _ => raise(Not_found)
      }
    | ChangeName(text) =>
      updateDefinition({
        ...definition,
        documentation: {
          ...definition.documentation,
          name: setTranslated(definition.documentation.name, "en", text),
        },
      })
    | ChangeDescription(text) =>
      updateDefinition({
        ...definition,
        documentation: {
          ...definition.documentation,
          name:
            setTranslated(definition.documentation.description, "en", text),
        },
      })
    | AddInput =>
      let nibID = randomID();
      updateDefinition({
        documentation: {
          ...definition.documentation,
          inputs:
            Belt.Map.String.set(
              definition.documentation.inputs,
              nibID,
              emptyTranslatable,
            ),
        },
        display: {
          ...definition.display,
          inputOrdering:
            List.append(definition.display.inputOrdering, [nibID]),
        },
        implementation:
          switch (definition.implementation) {
          | InterfaceImplementation(interface) =>
            InterfaceImplementation({
              ...interface,
              inputTypes:
                Belt.Map.String.set(
                  interface.inputTypes,
                  nibID,
                  PrimitiveValueType(TextType),
                ),
            })
          | GraphImplementation(graphImplementation) =>
            GraphImplementation({
              ...graphImplementation,
              interface: {
                ...graphImplementation.interface,
                inputTypes:
                  Belt.Map.String.set(
                    graphImplementation.interface.inputTypes,
                    nibID,
                    AnyType,
                  ),
              },
            })
          | RecordTypeImplementation(typedFields) =>
            RecordTypeImplementation(
              Belt.Map.String.set(
                typedFields,
                nibID,
                PrimitiveValueType(TextType),
              ),
            )
          | _ => definition.implementation
          },
      });
    | AddOutput =>
      let nibID = randomID();
      updateDefinition({
        documentation: {
          ...definition.documentation,
          outputs:
            Belt.Map.String.set(
              definition.documentation.outputs,
              nibID,
              emptyTranslatable,
            ),
        },
        display: {
          ...definition.display,
          outputOrdering:
            List.append(definition.display.outputOrdering, [nibID]),
        },
        implementation:
          switch (definition.implementation) {
          | InterfaceImplementation(interface) =>
            InterfaceImplementation({
              ...interface,
              outputTypes:
                Belt.Map.String.set(
                  interface.outputTypes,
                  nibID,
                  PrimitiveValueType(NumberType),
                ),
            })
          | GraphImplementation(graphImplementation) =>
            GraphImplementation({
              ...graphImplementation,
              interface: {
                ...graphImplementation.interface,
                outputTypes:
                  Belt.Map.String.set(
                    graphImplementation.interface.outputTypes,
                    nibID,
                    AnyType,
                  ),
              },
            })
          | _ => definition.implementation
          },
      });
    | NibAction({nibID, isInput, action}) =>
      switch (action) {
      | ChangeNibName(text) =>
        let nibs =
          isInput ?
            definition.documentation.inputs : definition.documentation.outputs;
        let nib = Belt.Map.String.getExn(nibs, nibID);
        let newNib = setTranslated(nib, "en", text);
        let newNibs = Belt.Map.String.set(nibs, nibID, newNib);
        let documentation =
          isInput ?
            {...definition.documentation, inputs: newNibs} :
            {...definition.documentation, outputs: newNibs};
        updateDefinition({...definition, documentation});
      | ChangeNibType(valueType) =>
        updateDefinition({
          ...definition,
          implementation:
            switch (definition.implementation) {
            | InterfaceImplementation(interface) =>
              InterfaceImplementation(
                changeInterface(interface, isInput, nibID, valueType),
              )
            | RecordTypeImplementation(typedFields) =>
              RecordTypeImplementation(
                isInput ?
                  changeTypedFields(typedFields, nibID, valueType) :
                  raise(Not_found),
              )
            | GraphImplementation(graphImplementation) =>
              GraphImplementation({
                ...graphImplementation,
                interface:
                  changeInterface(
                    graphImplementation.interface,
                    isInput,
                    nibID,
                    valueType,
                  ),
              })
            | _ => raise(Not_found)
            },
        })
      | ChangeNibOrdering(index) =>
        updateDefinition({
          ...definition,
          display:
            isInput ?
              {
                ...definition.display,
                inputOrdering:
                  moveToListIndex(
                    definition.display.inputOrdering,
                    nibID,
                    index,
                  ),
              } :
              {
                ...definition.display,
                outputOrdering:
                  moveToListIndex(
                    definition.display.outputOrdering,
                    nibID,
                    index,
                  ),
              },
        })
      | RemoveNib =>
        let uses =
          AffectedDefinitions.findConnectedDefinitions(
            definitionID,
            nibID,
            isInput,
            state.definitions,
          );
        if (Belt.Map.String.isEmpty(uses)) {
          /* TODO: remove nib from translations?  Unify "interface" usage? */
          updateDefinition({
            ...definition,
            display:
              isInput ?
                {
                  ...definition.display,
                  inputOrdering:
                    Belt.List.keep(definition.display.inputOrdering, item =>
                      item != nibID
                    ),
                } :
                {
                  ...definition.display,
                  outputOrdering:
                    Belt.List.keep(definition.display.outputOrdering, item =>
                      item != nibID
                    ),
                },
            implementation:
              switch (definition.implementation) {
              | GraphImplementation(graphImplementation) =>
                GraphImplementation({
                  ...graphImplementation,
                  connections:
                    Belt.Map.keep(
                      graphImplementation.connections, (sink, source) =>
                      !(
                        switch (isInput ? source : sink) {
                        | {
                            node: GraphConnection,
                            nib: NibConnection(connectionNibID),
                          } =>
                          connectionNibID == nibID
                        | _ => false
                        }
                      )
                    ),
                  interface:
                    removeInterfaceNib(
                      graphImplementation.interface,
                      nibID,
                      isInput,
                    ),
                })
              | InterfaceImplementation(interface) =>
                InterfaceImplementation(
                  removeInterfaceNib(interface, nibID, isInput),
                )
              | RecordTypeImplementation(typedFields) =>
                RecordTypeImplementation(
                  Belt.Map.String.keep(typedFields, (fieldNibID, _) =>
                    fieldNibID != nibID
                  ),
                )
              | other => other
              },
          });
        } else {
          ReasonReact.Update({...state, error: NibIsConnected(uses)});
        };
      }
    | RemoveConnection(connectionSide) =>
      updateDefinition({
        ...definition,
        implementation:
          switch (definition.implementation) {
          | GraphImplementation(graphImplementation) =>
            GraphImplementation({
              ...graphImplementation,
              connections:
                Belt.Map.remove(
                  graphImplementation.connections,
                  connectionSide,
                ),
            })
          | _ => raise(Not_found)
          },
      })
    | RemoveNodes(nodeIDs) =>
      updateDefinition({
        ...definition,
        implementation:
          switch (definition.implementation) {
          | GraphImplementation(graphImplementation) =>
            let nodeIDs =
              ExpandDeletion.getAffectedNodes(
                nodeIDs,
                graphImplementation.nodes,
              );
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
                    ExpandDeletion.connectionSideInvolvesNodeIDs(
                      sink,
                      nodeIDs,
                    )
                    || ExpandDeletion.connectionSideInvolvesNodeIDs(
                         source,
                         nodeIDs,
                       )
                  )
                ),
            });
          | _ => raise(Not_found)
          },
      })
    | Fork =>
      let newDefinitionID = randomID();
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          definitions:
            Belt.Map.String.set(
              state.definitions,
              newDefinitionID,
              definition,
            ),
        },
        _ => ReasonReact.Router.push("#" ++ newDefinitionID),
      );
    | ChangeNodeScope({nodeID, nodeScope}) =>
      switch (definition.implementation) {
      | GraphImplementation(graphImplementation) =>
        if (switch (nodeScope) {
            | GraphScope => true
            | NodeScope(scopeNodeID) =>
              isFunctionDefinitionNode(
                Belt.Map.String.getExn(
                  graphImplementation.nodes,
                  scopeNodeID,
                ),
              )
              && nodeID != scopeNodeID
            }) {
          let node =
            Belt.Map.String.getExn(graphImplementation.nodes, nodeID);
          let newNode = {...node, scope: nodeScope};
          let nodes =
            Belt.Map.String.set(graphImplementation.nodes, nodeID, newNode);
          if (DetectCycles.checkScopes(graphImplementation.connections, nodes)) {
            updateDefinition({
              ...definition,
              implementation:
                GraphImplementation({...graphImplementation, nodes}),
            });
          } else {
            ReasonReact.Update({
              ...state,
              error: ConnectionCrossesScopeError,
            });
          };
        } else {
          ReasonReact.NoUpdate;
        }

      | _ => ReasonReact.NoUpdate
      }
    };
  };
