[%%debugger.chrome];
let f =
    (action: AppAction.t, state: AppState.t)
    : ReactUpdate.update(AppAction.t, AppState.t) =>
  switch (action) {
  | CreateDefinition(definition) =>
    let newDefinitionID = RandomIDMake.f();
    ReactUpdate.UpdateWithSideEffects(
      {
        ...state,
        definitions:
          Belt.Map.String.set(state.definitions, newDefinitionID, definition),
      },
      _ => {
        ReasonReact.Router.push("#" ++ newDefinitionID);
        None;
      },
    );
  | DefinitionAction({definitionID, action}) =>
    let definition = Belt.Map.String.getExn(state.definitions, definitionID);

    let updateDefinition = definition =>
      ReactUpdate.Update({
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
      | _ => ReactUpdate.NoUpdate
      }

    | AddNode({node, explicitConnectionSide, connectionNib}) =>
      let nodeID = RandomIDMake.f();
      let nodeConnectionSide =
        ConnectionSide.{node: NodeConnection(nodeID), nib: connectionNib};
      let (source, sink) =
        explicitConnectionSide.isSource
          ? (explicitConnectionSide.connectionSide, nodeConnectionSide)
          : (nodeConnectionSide, explicitConnectionSide.connectionSide);
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
      | _ => ReactUpdate.NoUpdate
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
          name:
            TranslatableSetText.f(definition.documentation.name, "en", text),
        },
      })
    | ChangeDescription(text) =>
      updateDefinition({
        ...definition,
        documentation: {
          ...definition.documentation,
          name:
            TranslatableSetText.f(
              definition.documentation.description,
              "en",
              text,
            ),
        },
      })
    | AddInput =>
      let nibID = RandomIDMake.f();
      updateDefinition({
        documentation: {
          ...definition.documentation,
          inputs:
            Belt.Map.String.set(
              definition.documentation.inputs,
              nibID,
              TranslatableEmpty.v,
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
              input:
                Belt.Map.String.set(
                  interface.input,
                  nibID,
                  PrimitiveValueType(TextType),
                ),
            })
          | GraphImplementation(graphImplementation) =>
            GraphImplementation({
              ...graphImplementation,
              interface: {
                ...graphImplementation.interface,
                input:
                  Belt.Map.String.set(
                    graphImplementation.interface.input,
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
      let nibID = RandomIDMake.f();
      updateDefinition({
        documentation: {
          ...definition.documentation,
          outputs:
            Belt.Map.String.set(
              definition.documentation.outputs,
              nibID,
              TranslatableEmpty.v,
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
              output:
                Belt.Map.String.set(
                  interface.output,
                  nibID,
                  PrimitiveValueType(NumberType),
                ),
            })
          | GraphImplementation(graphImplementation) =>
            GraphImplementation({
              ...graphImplementation,
              interface: {
                ...graphImplementation.interface,
                output:
                  Belt.Map.String.set(
                    graphImplementation.interface.output,
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
          isInput
            ? definition.documentation.inputs
            : definition.documentation.outputs;
        let nib = Belt.Map.String.getExn(nibs, nibID);
        let newNib = TranslatableSetText.f(nib, "en", text);
        let newNibs = Belt.Map.String.set(nibs, nibID, newNib);
        let documentation =
          isInput
            ? {...definition.documentation, inputs: newNibs}
            : {...definition.documentation, outputs: newNibs};
        updateDefinition({...definition, documentation});
      | ChangeNibType(valueType) =>
        updateDefinition({
          ...definition,
          implementation:
            switch (definition.implementation) {
            | InterfaceImplementation(interface) =>
              InterfaceImplementation(
                InterfaceChange.f(interface, isInput, nibID, valueType),
              )
            | RecordTypeImplementation(typedFields) =>
              RecordTypeImplementation(
                isInput
                  ? RecordChange.f(typedFields, nibID, valueType)
                  : raise(Not_found),
              )
            | GraphImplementation(graphImplementation) =>
              GraphImplementation({
                ...graphImplementation,
                interface:
                  InterfaceChange.f(
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
            isInput
              ? {
                ...definition.display,
                inputOrdering:
                  ListMoveItemToIndex.f(
                    definition.display.inputOrdering,
                    nibID,
                    index,
                  ),
              }
              : {
                ...definition.display,
                outputOrdering:
                  ListMoveItemToIndex.f(
                    definition.display.outputOrdering,
                    nibID,
                    index,
                  ),
              },
        })
      | RemoveNib =>
        let uses =
          FindConnectedDefinitions.f(
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
              isInput
                ? {
                  ...definition.display,
                  inputOrdering:
                    Belt.List.keep(definition.display.inputOrdering, item =>
                      item != nibID
                    ),
                }
                : {
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
                    InterfaceRemoveNib.f(
                      graphImplementation.interface,
                      nibID,
                      isInput,
                    ),
                })
              | InterfaceImplementation(interface) =>
                InterfaceImplementation(
                  InterfaceRemoveNib.f(interface, nibID, isInput),
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
          ReactUpdate.Update(
            {
              {...state, error: NibIsConnected(uses)};
            },
          );
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
              DeletionGetAffectedNodes.f(nodeIDs, graphImplementation.nodes);
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
                    DeletionConnectionSideInvolvesNodeIDs.f(sink, nodeIDs)
                    || DeletionConnectionSideInvolvesNodeIDs.f(
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
      let newDefinitionID = RandomIDMake.f();
      ReactUpdate.UpdateWithSideEffects(
        {
          ...state,
          definitions:
            Belt.Map.String.set(
              state.definitions,
              newDefinitionID,
              definition,
            ),
        },
        _ => {
          ReasonReact.Router.push("#" ++ newDefinitionID);
          None;
        },
      );
    | ChangeNodeScope({nodeID, nodeScope}) =>
      switch (definition.implementation) {
      | GraphImplementation(graphImplementation) =>
        if (switch (nodeScope) {
            | GraphScope => true
            | NodeScope(scopeNodeID) =>
              NodeIsFunctionDefinition.f(
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
          if (ConnectionMapCheckScopes.f(
                graphImplementation.connections,
                nodes,
              )) {
            updateDefinition({
              ...definition,
              implementation:
                GraphImplementation({...graphImplementation, nodes}),
            });
          } else {
            ReactUpdate.Update(
              {
                {...state, error: ConnectionCrossesScopeError};
              },
            );
          };
        } else {
          ReactUpdate.NoUpdate;
        }
      | _ => ReactUpdate.NoUpdate
      }
    | EvaluateNib(explicitConnectionSide) =>
      let scopeID = RandomIDMake.f();
      ReactUpdate.Update({
        ...state,
        execution:
          Some({
            result: None,
            scopes:
              Belt.Map.String.fromArray([|
                (scopeID, ScopeMake.f(definitionID, None)),
              |]),
            stack: [{scopeID, explicitConnectionSide, action: Evaluating}],
          }),
      });
    };

  | Step =>
    ReactUpdate.Update({
      ...state,
      execution:
        switch (state.execution) {
        | None => None
        | Some(execution) =>
          let frame = Belt.List.headExn(execution.stack);

          switch (
            Belt.Map.String.getExn(execution.scopes, frame.scopeID).
              parentScope
          ) {
          | Some(_) => Js.log("Has a parent scope")
          | None => Js.log("no parent scope")
          };

          Some(
            switch (frame.action) {
            | Evaluating => ExecutionReducer.f(execution, state.definitions)
            | Returning(value) =>
              if (Belt.List.length(execution.stack) == 1) {
                {...execution, result: Some(value)};
              } else {
                Js.log("big enough stack");
                let defaultBehavior = () =>
                  ExecutionReducer.f(
                    {
                      ...execution,
                      stack: Belt.List.tailExn(execution.stack),
                    },
                    state.definitions,
                  );
                switch (frame.explicitConnectionSide.connectionSide.node) {
                | GraphConnection => defaultBehavior()
                | NodeConnection(nodeID) =>
                  Js.log2("it's a node", nodeID);
                  let scope =
                    Belt.Map.String.getExn(execution.scopes, frame.scopeID);
                  let definition =
                    Belt.Map.String.getExn(
                      state.definitions,
                      scope.definitionID,
                    );
                  switch (definition.implementation) {
                  | GraphImplementation(graphImplementation) =>
                    Js.log("we're in a graph");
                    let node =
                      Belt.Map.String.getExn(
                        graphImplementation.nodes,
                        nodeID,
                      );
                    switch (node.kind) {
                    | DefinedNode({kind, definitionID}) =>
                      Js.log("it's a defined node");
                      let nodeDefinition =
                        Belt.Map.String.getExn(
                          state.definitions,
                          definitionID,
                        );
                      switch (kind) {
                      | FunctionCallNode =>
                        Js.log("it's a function call node");
                        switch (nodeDefinition.implementation) {
                        | GraphImplementation(_) =>
                          Js.log("it has a graph implementation");
                          let scope =
                            Belt.Map.String.getExn(
                              execution.scopes,
                              frame.scopeID,
                            );
                          let nodeScopeID =
                            Belt.Map.String.getExn(
                              scope.nodeScopeIDs,
                              nodeID,
                            );
                          let connectionSide =
                            ConnectionSide.{
                              node: GraphConnection,
                              nib:
                                frame.explicitConnectionSide.connectionSide.
                                  nib,
                            };
                          {
                            ...execution,
                            stack: [
                              {
                                scopeID: nodeScopeID,
                                explicitConnectionSide: {
                                  isSource: true,
                                  connectionSide,
                                },
                                action: Returning(value),
                              },
                              ...Belt.List.tailExn(execution.stack),
                            ],
                            scopes:
                              MapStringUpdate.f(
                                execution.scopes,
                                nodeScopeID,
                                (scope: Scope.t) =>
                                {
                                  ...scope,
                                  sourceValues:
                                    Belt.Map.set(
                                      scope.sourceValues,
                                      connectionSide,
                                      value,
                                    ),
                                }
                              ),
                          };
                        | _ => defaultBehavior()
                        };
                      | _ => defaultBehavior()
                      };
                    | _ => defaultBehavior()
                    };
                  | _ => defaultBehavior()
                  };
                };
              }
            },
          );
        },
    })
  };
