let quickReturns = true
let rec f = (state: AppState.t, webView, urlHash): ReactUpdate.update<AppAction.t, AppState.t> => {
  let definitions = state.definitions
  switch state.execution {
  | None => ReactUpdate.NoUpdate
  | Some(execution) =>
    if Belt.List.length(execution.stack) == 0 {
      ReactUpdate.NoUpdate
    } else {
      let frame = Belt.List.headExn(execution.stack)
      switch frame.action {
      | Returning =>
        if Belt.List.length(execution.stack) == 1 {
          // The execution stack is empty.  Try to resolve the result now.
          let lazies = ValueGetLazies.f(execution.result, execution, state.definitions)
          ReactUpdate.UpdateWithSideEffects(
            {
              ...state,
              execution: Some({
                ...execution,
                stack: Belt.List.map(lazies, lazyValue => {
                  open StackFrame
                  {
                    scopeID: lazyValue.scopeID,
                    explicitConnectionSide: lazyValue.explicitConnectionSide,
                    action: EvaluationAction.Evaluating,
                  }
                }),
              }),
            },
            ExecutionReducerSideEffects.f(urlHash),
          )
        } else {
          // Pop the Return off the stack.
          let newState = {
            ...state,
            execution: Some({
              ...execution,
              stack: Belt.List.tailExn(execution.stack),
            }),
          }
          if quickReturns {
            f(newState, webView, urlHash)
          } else {
            ReactUpdate.UpdateWithSideEffects(newState, ExecutionReducerSideEffects.f(urlHash))
          }
        }
      | Evaluating =>
        let scope = Belt.Map.String.getExn(execution.scopes, frame.scopeID)
        let graphDefinitionID = ScopeGetGraphDefinitionID.f(execution.scopes, frame.scopeID)
        let definition = Belt.Map.String.getExn(definitions, graphDefinitionID)
        switch definition.implementation {
        | GraphImplementation(graphImplementation) =>
          let source = ExplicitConnectionSideGetSource.f(
            frame.explicitConnectionSide,
            graphImplementation.connections,
          )
          // check the cache to see if it's already evaluated
          switch ValueResolve.resolveSource(scope, source, execution.scopes, definitions) {
          | Some(Value.LazyValue(lazyValue)) =>
            // Require evaluation
            let stackFrame: StackFrame.t = {
              scopeID: lazyValue.scopeID,
              explicitConnectionSide: lazyValue.explicitConnectionSide,
              action: Evaluating,
            }
            UpdateExecution.f(
              state,
              urlHash,
              {
                ...execution,
                stack: list{stackFrame, ...execution.stack},
              },
            )
          | Some(_) =>
            // Return
            ReactUpdate.UpdateWithSideEffects(
              {
                ...state,
                execution: Some({
                  ...execution,
                  stack: list{{...frame, action: Returning}, ...Belt.List.tailExn(execution.stack)},
                }),
              },
              ExecutionReducerSideEffects.f(urlHash),
            )
          | None =>
            switch source.node {
            | NodeConnection(nodeID) =>
              let node = Belt.Map.String.getExn(graphImplementation.nodes, nodeID)
              switch node.kind {
              | DefinedNode({kind, definitionID}) =>
                let nodeDefinition = Belt.Map.String.getExn(definitions, definitionID)
                switch kind {
                | ValueNode => {
                    let value = switch nodeDefinition.implementation {
                    | ConstantImplementation(primitiveValue) => Value.PrimitiveValue(primitiveValue)
                    | SymbolImplementation =>
                      Value.DefinedValue({definitionID: definitionID, value: SymbolValue})
                    // TODO: disallow creation of these nodes since they can't be used currently
                    | ExternalImplementation(_)
                    | GraphImplementation(_) =>
                      Value.DefinedValue({definitionID: definitionID, value: FunctionPointerValue})
                    | _ =>
                      raise(
                        Exception.TODO("Evaluating a ValueNode that's not a symbol or constant"),
                      )
                    }
                    ExecutionReducerReturn.f(value, execution, source, state, urlHash)
                  }
                | FunctionCallNode =>
                  switch nodeDefinition.implementation {
                  | ExternalImplementation(externalImplementation) =>
                    switch source.nib {
                    | NibConnection(outputID) =>
                      EvaluateExternalReducer.f(
                        execution,
                        definitions,
                        scope,
                        frame,
                        source,
                        graphImplementation.connections,
                        externalImplementation,
                        outputID,
                        state,
                        webView,
                        urlHash,
                      )
                    | _ =>
                      raise(
                        Exception.ShouldntHappen(
                          "Evaluating a non-keyword nib on an external function",
                        ),
                      )
                    }
                  | GraphImplementation(_) =>
                    let (nodeScopeID, scopes) = switch Belt.Map.String.get(
                      scope.nodeScopeIDs,
                      nodeID,
                    ) {
                    | Some(nodeScopeID) => (nodeScopeID, execution.scopes)
                    | None =>
                      let nodeScopeID = RandomIDMake.f()
                      (
                        nodeScopeID,
                        Belt.Map.String.set(
                          Belt.Map.String.set(
                            execution.scopes,
                            nodeScopeID,
                            ScopeMake.f(
                              definitionID,
                              Some({callingScopeID: frame.scopeID, nodeID: nodeID}),
                              GraphScope,
                            ),
                          ),
                          frame.scopeID,
                          {
                            ...scope,
                            nodeScopeIDs: Belt.Map.String.set(
                              scope.nodeScopeIDs,
                              nodeID,
                              nodeScopeID,
                            ),
                          },
                        ),
                      )
                    }

                    ReactUpdate.UpdateWithSideEffects(
                      {
                        ...state,
                        execution: Some({
                          ...execution,
                          scopes: scopes,
                          stack: list{
                            {
                              action: Evaluating,
                              scopeID: nodeScopeID,
                              explicitConnectionSide: {
                                isSource: false,
                                connectionSide: {
                                  node: GraphConnection,
                                  nib: source.nib,
                                },
                              },
                            },
                            ...execution.stack,
                          },
                        }),
                      },
                      ExecutionReducerSideEffects.f(urlHash),
                    )
                  | _ =>
                    raise(Exception.TODO("Evaluating a function that's not a graph or external"))
                  }
                | ConstructorNode =>
                  switch nodeDefinition.implementation {
                  | RecordTypeImplementation(typedFields) =>
                    let value = Value.DefinedValue({
                      definitionID: definitionID,
                      value: RecordValue(
                        Belt.Map.String.mapWithKey(typedFields, (nibID, _) => Value.LazyValue({
                          scopeID: frame.scopeID,
                          explicitConnectionSide: {
                            isSource: false,
                            connectionSide: {
                              node: NodeConnection(nodeID),
                              nib: NibConnection(nibID),
                            },
                          },
                        })),
                      ),
                    })
                    ExecutionReducerReturn.f(value, execution, source, state, urlHash)
                  | LabeledTypeImplementation(_) =>
                    let value = Value.DefinedValue({
                      definitionID: definitionID,
                      value: LabeledValue(
                        LazyValue({
                          scopeID: frame.scopeID,
                          explicitConnectionSide: {
                            isSource: false,
                            connectionSide: {
                              node: NodeConnection(nodeID),
                              nib: ValueConnection,
                            },
                          },
                        }),
                      ),
                    })
                    ExecutionReducerReturn.f(value, execution, source, state, urlHash)
                  | _ =>
                    raise(
                      Exception.ShouldntHappen(
                        "Evaluating a constructor that's not a record or label",
                      ),
                    )
                  }
                | AccessorNode =>
                  switch nodeDefinition.implementation {
                  | RecordTypeImplementation(_) =>
                    switch Belt.Map.get(
                      scope.sourceValues,
                      Belt.Map.getExn(
                        graphImplementation.connections,
                        {node: source.node, nib: ValueConnection},
                      ),
                    ) {
                    | Some(value) =>
                      switch source.nib {
                      | NibConnection(nibID) =>
                        switch value {
                        | DefinedValue(definedValue) =>
                          switch definedValue.value {
                          | RecordValue(values) =>
                            switch Belt.Map.String.getExn(values, nibID) {
                            | LazyValue(lazyValue) =>
                              switch ValueResolve.resolveLazyValue(
                                lazyValue,
                                definitions,
                                execution.scopes,
                              ) {
                              | None =>
                                // evaluate lazy value
                                Js.log("None value")
                                ReactUpdate.UpdateWithSideEffects(
                                  {
                                    ...state,
                                    execution: Some({
                                      ...execution,
                                      stack: list{
                                        {
                                          open StackFrame
                                          {
                                            scopeID: lazyValue.scopeID,
                                            explicitConnectionSide: lazyValue.explicitConnectionSide,
                                            action: EvaluationAction.Evaluating,
                                          }
                                        },
                                        ...execution.stack,
                                      },
                                    }),
                                  },
                                  ExecutionReducerSideEffects.f(urlHash),
                                )

                              | Some(value) =>
                                ExecutionReducerReturn.f(value, execution, source, state, urlHash)
                              }
                            | _ => raise(Not_found) // todo
                            }
                          | _ => raise(Not_found)
                          }
                        | _ => raise(Not_found)
                        }
                      | _ => raise(Not_found)
                      }

                    | None =>
                      ReactUpdate.UpdateWithSideEffects(
                        {
                          ...state,
                          execution: Some({
                            ...execution,
                            stack: list{
                              {
                                ...frame,
                                action: Evaluating,
                                explicitConnectionSide: {
                                  isSource: false,
                                  connectionSide: {
                                    node: source.node,
                                    nib: ValueConnection,
                                  },
                                },
                              },
                              ...execution.stack,
                            },
                          }),
                        },
                        ExecutionReducerSideEffects.f(urlHash),
                      )
                    }
                  | LabeledTypeImplementation(_) =>
                    switch Belt.Map.get(
                      scope.sourceValues,
                      Belt.Map.getExn(
                        graphImplementation.connections,
                        {node: source.node, nib: ValueConnection},
                      ),
                    ) {
                    | Some(value) =>
                      switch value {
                      | DefinedValue(definedValue) =>
                        switch definedValue.value {
                        | LabeledValue(wrappedValue) =>
                          switch wrappedValue {
                          | LazyValue(lazyValue) =>
                            switch ValueResolve.resolveLazyValue(
                              lazyValue,
                              definitions,
                              execution.scopes,
                            ) {
                            | None =>
                              Js.log("None value")
                              ReactUpdate.UpdateWithSideEffects(
                                {
                                  ...state,
                                  execution: Some({
                                    ...execution,
                                    stack: list{
                                      {
                                        open StackFrame
                                        {
                                          scopeID: lazyValue.scopeID,
                                          explicitConnectionSide: lazyValue.explicitConnectionSide,
                                          action: EvaluationAction.Evaluating,
                                        }
                                      },
                                      ...execution.stack,
                                    },
                                  }),
                                },
                                ExecutionReducerSideEffects.f(urlHash),
                              )

                            | Some(value) =>
                              ExecutionReducerReturn.f(value, execution, source, state, urlHash)
                            }
                          | _ => raise(Not_found) // todo
                          }
                        | _ => raise(Not_found)
                        }
                      | _ => raise(Not_found)
                      }

                    | None =>
                      ReactUpdate.UpdateWithSideEffects(
                        {
                          ...state,
                          execution: Some({
                            ...execution,
                            stack: list{
                              {
                                ...frame,
                                action: Evaluating,
                                explicitConnectionSide: {
                                  isSource: false,
                                  connectionSide: {
                                    node: source.node,
                                    nib: ValueConnection,
                                  },
                                },
                              },
                              ...execution.stack,
                            },
                          }),
                        },
                        ExecutionReducerSideEffects.f(urlHash),
                      )
                    }
                  | _ =>
                    raise(
                      Exception.ShouldntHappen(
                        "Evaluating an accessor that's not a record or label",
                      ),
                    )
                  }
                | FunctionDefinitionNode =>
                  switch source.nib {
                  | ValueConnection =>
                    let value = Value.InlineFunction({parentScopeID: frame.scopeID, nodeID: nodeID})
                    ExecutionReducerReturn.f(value, execution, source, state, urlHash)
                  | NibConnection(_nibID) =>
                    switch scope.callingContext {
                    | Some(callingContext) =>
                      ReactUpdate.UpdateWithSideEffects(
                        {
                          ...state,
                          execution: Some({
                            ...execution,
                            stack: list{
                              {
                                open StackFrame
                                {
                                  scopeID: callingContext.callingScopeID,
                                  explicitConnectionSide: {
                                    isSource: false,
                                    connectionSide: {
                                      node: NodeConnection(callingContext.nodeID),
                                      nib: source.nib,
                                    },
                                  },
                                  action: Evaluating,
                                }
                              },
                              ...execution.stack,
                            },
                          }),
                        },
                        ExecutionReducerSideEffects.f(urlHash),
                      )

                    | None =>
                      raise(Exception.ShouldntHappen("Inline function definition had no caller."))
                    }
                  | _ =>
                    raise(
                      Exception.ShouldntHappen(
                        "Inline function definitions don't have positional arguments.",
                      ),
                    )
                  }
                | FunctionPointerCallNode =>
                  switch nodeDefinition.implementation {
                  | InterfaceImplementation(_interfaceImplementation) =>
                    let implementationNib: ConnectionSide.t = {
                      node: source.node,
                      nib: ValueConnection,
                    }
                    let implementationSource = Belt.Map.getExn(
                      graphImplementation.connections,
                      implementationNib,
                    )
                    switch ValueResolve.resolveSource(
                      scope,
                      implementationSource,
                      execution.scopes,
                      definitions,
                    ) {
                    | None =>
                      let stackFrame: StackFrame.t = {
                        scopeID: frame.scopeID,
                        explicitConnectionSide: {
                          isSource: false,
                          connectionSide: implementationNib,
                        },
                        action: Evaluating,
                      }
                      UpdateExecution.f(
                        state,
                        urlHash,
                        {
                          ...execution,
                          stack: list{stackFrame, ...execution.stack},
                        },
                      )

                    | Some(InlineFunction({parentScopeID, nodeID: inlineNodeID})) =>
                      // first, we check if the function has been called by looking at the node scope for the calling node.
                      let (nodeScopeID, scopes) = switch Belt.Map.String.get(
                        scope.nodeScopeIDs,
                        nodeID,
                      ) {
                      | Some(nodeScopeID) => (nodeScopeID, execution.scopes)
                      | None =>
                        let nodeScopeID = RandomIDMake.f()

                        let calledScope = ScopeMake.f(
                          definitionID,
                          Some({callingScopeID: frame.scopeID, nodeID: nodeID}),
                          InlineScope({nodeID: inlineNodeID, parentScopeID: parentScopeID}),
                        )
                        let scopes = Belt.Map.String.set(execution.scopes, nodeScopeID, calledScope)

                        let callingScope = {
                          ...scope,
                          nodeScopeIDs: Belt.Map.String.set(
                            scope.nodeScopeIDs,
                            nodeID,
                            nodeScopeID,
                          ),
                        }
                        let scopes = Belt.Map.String.set(scopes, frame.scopeID, callingScope)

                        (nodeScopeID, scopes)
                      }

                      let newFrame: StackFrame.t = {
                        action: Evaluating,
                        scopeID: nodeScopeID,
                        explicitConnectionSide: {
                          isSource: false,
                          connectionSide: {
                            node: NodeConnection(inlineNodeID), // TODO: get the node
                            nib: source.nib,
                          },
                        },
                      }
                      UpdateExecution.f(
                        state,
                        urlHash,
                        {
                          ...execution,
                          scopes: scopes,
                          stack: list{newFrame, ...execution.stack},
                        },
                      )

                    | _ =>
                      raise(
                        Exception.ShouldntHappen(
                          "Inline function call received something other than an inline function definition.",
                        ),
                      )
                    }
                  | _ => raise(Exception.TODO("Inline function call wasn't an interface."))
                  }
                }
              | _ => raise(Exception.TODO("Evaluating a new kind of node"))
              }
            | GraphConnection =>
              switch scope.callingContext {
              | Some(callingContext) =>
                ReactUpdate.UpdateWithSideEffects(
                  {
                    ...state,
                    execution: Some({
                      ...execution,
                      stack: list{
                        {
                          open StackFrame
                          {
                            scopeID: callingContext.callingScopeID,
                            explicitConnectionSide: {
                              isSource: false,
                              connectionSide: {
                                node: NodeConnection(callingContext.nodeID),
                                nib: source.nib,
                              },
                            },
                            action: Evaluating,
                          }
                        },
                        ...execution.stack,
                      },
                    }),
                  },
                  ExecutionReducerSideEffects.f(urlHash),
                )

              | None =>
                // todo: a real implementation
                let value = Value.PrimitiveValue(NumberValue(3.0))
                ExecutionReducerReturn.f(value, execution, source, state, urlHash)
              }
            }
          }

        | _ => raise(Exception.TODO("Evaluating a non-graph"))
        }
      }
    }
  }
}
