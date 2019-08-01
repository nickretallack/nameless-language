[%%debugger.chrome];
open Definition;
open! AppActions;
open Helpers;

type appState = {
  execution: option(execution),
  definitions,
  definitionID,
  error: appError,
};

let evaluate = (execution: execution, definitions: definitions): execution => {
  let frame = Belt.List.headExn(execution.stack);
  let scope = Belt.Map.String.getExn(execution.scopes, frame.scopeID);
  let definition = Belt.Map.String.getExn(definitions, scope.definitionID);
  switch (definition.implementation) {
  | GraphImplementation(graphImplementation) =>
    let source =
      frame.explicitConnectionSide.isSource ?
        frame.explicitConnectionSide.connectionSide :
        Belt.Map.getExn(
          graphImplementation.connections,
          frame.explicitConnectionSide.connectionSide,
        );
    switch (source.node) {
    | NodeConnection(nodeID) =>
      let node = Belt.Map.String.getExn(graphImplementation.nodes, nodeID);
      switch (node.kind) {
      | DefinedNode({kind, definitionID}) =>
        let nodeDefinition =
          Belt.Map.String.getExn(definitions, definitionID);
        switch (kind) {
        | ValueNode =>
          switch (nodeDefinition.implementation) {
          | ConstantImplementation(primitiveValue) =>
            let value = PrimitiveValue(primitiveValue);
            {
              ...execution,
              stack: [
                {...frame, action: Returning(value)},
                ...Belt.List.tailExn(execution.stack),
              ],
              scopes:
                Belt.Map.String.set(
                  execution.scopes,
                  frame.scopeID,
                  {
                    ...scope,
                    sourceValues:
                      Belt.Map.set(scope.sourceValues, source, value),
                  },
                ),
            };

          | _ => raise(Not_found) // todo
          }
        | FunctionCallNode =>
          switch (nodeDefinition.implementation) {
          | ExternalImplementation({name, interface}) =>
            switch (source.nib) {
            | NibConnection(outputID) =>
              switch (
                External.evaluateExternal(
                  name,
                  outputID,
                  Belt.Map.String.mapWithKey(interface.inputTypes, (nibID, _) =>
                    Belt.Map.get(
                      scope.sourceValues,
                      Belt.Map.getExn(
                        graphImplementation.connections,
                        {node: source.node, nib: NibConnection(nibID)},
                      ),
                    )
                  ),
                )
              ) {
              | EvaluationResult(value) => {
                  ...execution,
                  stack: [
                    {...frame, action: Returning(value)},
                    ...Belt.List.tailExn(execution.stack),
                  ],
                  scopes:
                    Belt.Map.String.set(
                      execution.scopes,
                      frame.scopeID,
                      {
                        ...scope,
                        sourceValues:
                          Belt.Map.set(scope.sourceValues, source, value),
                      },
                    ),
                }
              | EvaluationRequired(nibIDs) => {
                  ...execution,
                  stack:
                    Belt.List.concat(
                      Belt.List.map(nibIDs, nibID =>
                        {
                          ...frame,
                          explicitConnectionSide: {
                            isSource: false,
                            connectionSide: {
                              node: source.node,
                              nib: NibConnection(nibID),
                            },
                          },
                          action: Evaluating,
                        }
                      ),
                      execution.stack,
                    ),
                }
              }
            | _ => raise(Not_found)
            }
          | GraphImplementation(_) => raise(Not_found) // todo
          | _ => raise(Not_found) // todo
          }
        | ConstructorNode =>
          switch (nodeDefinition.implementation) {
          | RecordTypeImplementation(typedFields) =>
            let value =
              DefinedValue({
                definitionID,
                values:
                  Belt.Map.String.mapWithKey(typedFields, (nibID, _) =>
                    LazyValue({
                      ...frame,
                      action: Evaluating,
                      explicitConnectionSide: {
                        isSource: false,
                        connectionSide: {
                          node: NodeConnection(nodeID),
                          nib: NibConnection(nibID),
                        },
                      },
                    })
                  ),
              });
            {
              ...execution,
              scopes:
                Belt.Map.String.set(
                  execution.scopes,
                  frame.scopeID,
                  {
                    ...scope,
                    sourceValues:
                      Belt.Map.set(scope.sourceValues, source, value),
                  },
                ),
            };
          | _ => raise(Not_found)
          }
        | AccessorNode =>
          switch (nodeDefinition.implementation) {
          | RecordTypeImplementation(_) => raise(Not_found)
          | _ => raise(Not_found)
          }
        | _ => raise(Not_found) // todo
        };
      | _ => raise(Not_found) // todo
      };
    | GraphConnection =>
      // todo: a real implementation
      let value = PrimitiveValue(NumberValue(3.0));
      {
        ...execution,
        stack: [
          {...frame, action: Returning(value)},
          ...Belt.List.tailExn(execution.stack),
        ],
        scopes:
          Belt.Map.String.set(
            execution.scopes,
            frame.scopeID,
            {
              ...scope,
              sourceValues: Belt.Map.set(scope.sourceValues, source, value),
            },
          ),
      };
    };
  | _ => raise(Not_found) // todo
  };
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
    | EvaluateNib(explicitConnectionSide) =>
      let scopeID = randomID();
      ReasonReact.Update({
        ...state,
        execution:
          Some({
            result: None,
            scopes:
              Belt.Map.String.fromArray([|
                (
                  scopeID,
                  {
                    definitionID,
                    sourceValues:
                      Belt.Map.make(~id=(module ConnectionComparator)),
                  },
                ),
              |]),
            stack: [{scopeID, explicitConnectionSide, action: Evaluating}],
          }),
      });
    };
  | Step =>
    ReasonReact.Update({
      ...state,
      execution:
        switch (state.execution) {
        | None => None
        | Some(execution) =>
          let frame = Belt.List.headExn(execution.stack);
          Some(
            switch (frame.action) {
            | Evaluating => evaluate(execution, state.definitions)
            | Returning(value) =>
              if (Belt.List.length(execution.stack) == 1) {
                {...execution, result: Some(value)};
              } else {
                let stack = Belt.List.tailExn(execution.stack);
                let execution = {...execution, stack};
                evaluate(execution, state.definitions);
              }
            },
          );
        },
    })
  };
