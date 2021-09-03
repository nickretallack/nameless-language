// TODO: honor NodeScope.  Prevent scoped nodes' nib values from being stored in the graph's sourceValues.
let f = (execution: Execution.t, definitions: DefinitionMap.t, webView): Execution.t => {
  let frame = Belt.List.headExn(execution.stack)
  let scope = Belt.Map.String.getExn(execution.scopes, frame.scopeID)
  let graphDefinitionID = ScopeGetGraphDefinitionID.f(execution, frame.scopeID)
  let definition = Belt.Map.String.getExn(definitions, graphDefinitionID)
  switch definition.implementation {
  | GraphImplementation(graphImplementation) =>
    let source = ExplicitConnectionSideGetSource.f(
      frame.explicitConnectionSide,
      graphImplementation.connections,
    )
    // check the cache to see if it's already evaluated
    switch Belt.Map.get(scope.sourceValues, source) {
    | Some(LazyValue(lazyValue)) =>
      // evaluate lazy value
      switch LazyValueResolve.f(lazyValue, definitions, execution.scopes) {
      | None => {
          ...execution,
          stack: list{
            {
              open StackFrame
              {
                scopeID: lazyValue.scopeID,
                explicitConnectionSide: {
                  isSource: false,
                  connectionSide: lazyValue.connectionSide,
                },
                action: EvaluationAction.Evaluating,
              }
            },
            ...execution.stack,
          },
          scopes: execution.scopes,
        }
      | Some(value) => {
          ...execution,
          stack: list{{...frame, action: Returning(value)}, ...Belt.List.tailExn(execution.stack)},
        }
      }
    | Some(value) => {
        ...execution,
        stack: list{{...frame, action: Returning(value)}, ...Belt.List.tailExn(execution.stack)},
      }
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
              | _ => raise(Exception.TODO("Evaluating a ValueNode that's not a symbol or constant"))
              }
              ExecutionReducerReturn.f(value, execution, source)
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
                  webView,
                )
              | _ =>
                raise(
                  Exception.ShouldntHappen("Evaluating a non-keyword nib on an external function"),
                )
              }
            | GraphImplementation(_) =>
              let (nodeScopeID, scopes) = switch Belt.Map.String.get(scope.nodeScopeIDs, nodeID) {
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
                        Some({scopeID: frame.scopeID, nodeID: nodeID}),
                        GraphScope,
                      ),
                    ),
                    frame.scopeID,
                    {
                      ...scope,
                      nodeScopeIDs: Belt.Map.String.set(scope.nodeScopeIDs, nodeID, nodeScopeID),
                    },
                  ),
                )
              }

              {
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
              }
            | _ => raise(Exception.TODO("Evaluating a function that's not a graph or external"))
            }
          | ConstructorNode =>
            switch nodeDefinition.implementation {
            | RecordTypeImplementation(typedFields) =>
              let value = Value.DefinedValue({
                definitionID: definitionID,
                value: RecordValue(
                  Belt.Map.String.mapWithKey(typedFields, (nibID, _) => Value.LazyValue({
                    scopeID: frame.scopeID,
                    connectionSide: {
                      node: NodeConnection(nodeID),
                      nib: NibConnection(nibID),
                    },
                  })),
                ),
              })
              ExecutionReducerReturn.f(value, execution, source)
            | LabeledTypeImplementation(_) =>
              let value = Value.DefinedValue({
                definitionID: definitionID,
                value: LabeledValue(
                  LazyValue({
                    scopeID: frame.scopeID,
                    connectionSide: {
                      node: NodeConnection(nodeID),
                      nib: ValueConnection,
                    },
                  }),
                ),
              })
              ExecutionReducerReturn.f(value, execution, source)
            | _ =>
              raise(
                Exception.ShouldntHappen("Evaluating a constructor that's not a record or label"),
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
                        switch LazyValueResolve.f(lazyValue, definitions, execution.scopes) {
                        | None =>
                          // evaluate lazy value
                          Js.log("None value")
                          {
                            ...execution,
                            stack: list{
                              {
                                open StackFrame
                                {
                                  scopeID: lazyValue.scopeID,
                                  explicitConnectionSide: {
                                    isSource: false,
                                    connectionSide: lazyValue.connectionSide,
                                  },
                                  action: EvaluationAction.Evaluating,
                                }
                              },
                              ...execution.stack,
                            },
                            scopes: execution.scopes,
                          }
                        | Some(value) => {
                            ...execution,
                            stack: list{
                              {...frame, action: Returning(value)},
                              ...Belt.List.tailExn(execution.stack),
                            },
                            scopes: execution.scopes,
                          }
                        }
                      | _ => raise(Not_found) // todo
                      }
                    | _ => raise(Not_found)
                    }
                  | _ => raise(Not_found)
                  }
                | _ => raise(Not_found)
                }

              | None => {
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
                  scopes: execution.scopes,
                }
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
                      switch LazyValueResolve.f(lazyValue, definitions, execution.scopes) {
                      | None =>
                        Js.log("None value")
                        {
                          ...execution,
                          stack: list{
                            {
                              open StackFrame
                              {
                                scopeID: lazyValue.scopeID,
                                explicitConnectionSide: {
                                  isSource: false,
                                  connectionSide: lazyValue.connectionSide,
                                },
                                action: EvaluationAction.Evaluating,
                              }
                            },
                            ...execution.stack,
                          },
                          scopes: execution.scopes,
                        }
                      | Some(value) => {
                          ...execution,
                          stack: list{
                            {...frame, action: Returning(value)},
                            ...Belt.List.tailExn(execution.stack),
                          },
                          scopes: execution.scopes,
                        }
                      }
                    | _ => raise(Not_found) // todo
                    }
                  | _ => raise(Not_found)
                  }
                | _ => raise(Not_found)
                }

              | None => {
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
                  scopes: execution.scopes,
                }
              }
            | _ =>
              raise(Exception.ShouldntHappen("Evaluating an accessor that's not a record or label"))
            }
          | FunctionDefinitionNode =>
            switch source.nib {
            | ValueConnection =>
              let value = Value.InlineFunction({scopeID: frame.scopeID, nodeID: nodeID})
              ExecutionReducerReturn.f(value, execution, source)
            | NibConnection(nibID) =>
              switch scope.callingScope {
              | Some(callingScope) => {
                  ...execution,
                  stack: list{
                    {
                      open StackFrame
                      {
                        scopeID: callingScope.scopeID,
                        explicitConnectionSide: {
                          isSource: false,
                          connectionSide: {
                            node: NodeConnection(callingScope.nodeID),
                            nib: source.nib,
                          },
                        },
                        action: Evaluating,
                      }
                    },
                    ...execution.stack,
                  },
                }
              | None => raise(Exception.ShouldntHappen("Inline function definition had no caller."))
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
              switch Belt.Map.get(scope.sourceValues, implementationSource) {
              | None => {
                  ...execution,
                  stack: list{
                    {
                      ...frame,
                      explicitConnectionSide: {
                        isSource: false,
                        connectionSide: implementationNib,
                      },
                      action: Evaluating,
                    },
                    ...execution.stack,
                  },
                }
              | Some(InlineFunction({scopeID, nodeID: inlineNodeID})) =>
                // let inlineFunctionNode = Belt.Map.String.getExn(graphImplementation.nodes, nodeID)
                let inlineFunctionScope = Belt.Map.String.getExn(execution.scopes, scopeID)

                // try to evaluate this nib
                let inlineFunctionScopeDefinition = Belt.Map.String.getExn(
                  definitions,
                  inlineFunctionScope.definitionID,
                )
                let inlineSource = switch inlineFunctionScopeDefinition.implementation {
                | GraphImplementation(inlineFunctionGraphImplementation) =>
                  Belt.Map.getExn(
                    inlineFunctionGraphImplementation.connections,
                    {
                      node: NodeConnection(inlineNodeID),
                      nib: source.nib,
                    },
                  )
                | _ =>
                  raise(
                    Exception.ShouldntHappen("inline function definition's scope must be a graph"),
                  )
                }
                switch Belt.Map.get(inlineFunctionScope.sourceValues, inlineSource) {
                | Some(value) => ExecutionReducerReturn.f(value, execution, source)
                | None =>
                  // create a new scope
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
                            Some({scopeID: frame.scopeID, nodeID: nodeID}),
                            InlineScope({nodeID: inlineNodeID, parentScope: scopeID}),
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

                  // // make lazy values
                  // let newSourceValues = Belt.Map.String.reduce(
                  //   interfaceImplementation.input,
                  //   inlineFunctionScope.sourceValues,
                  //   (sourceValues, nibID, _) => {
                  //     let nib = ConnectionNib.NibConnection(nibID)
                  //     Belt.Map.set(
                  //       sourceValues,
                  //       {node: NodeConnection(nodeID), nib: nib},
                  //       Value.LazyValue({
                  //         scopeID: frame.scopeID,
                  //         connectionSide: {node: source.node, nib: nib},
                  //       }),
                  //     )
                  //   },
                  // )
                  {
                    ...execution,
                    scopes: scopes,
                    stack: list{
                      {
                        action: Evaluating,
                        scopeID: nodeScopeID,
                        explicitConnectionSide: {
                          isSource: false,
                          connectionSide: {
                            node: NodeConnection(inlineNodeID),
                            nib: source.nib,
                          },
                        },
                      },
                      ...execution.stack,
                    },
                  }
                }
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
        switch scope.callingScope {
        | Some(callingScope) => {
            ...execution,
            stack: list{
              {
                open StackFrame
                {
                  scopeID: callingScope.scopeID,
                  explicitConnectionSide: {
                    isSource: false,
                    connectionSide: {
                      node: NodeConnection(callingScope.nodeID),
                      nib: source.nib,
                    },
                  },
                  action: Evaluating,
                }
              },
              ...execution.stack,
            },
          }
        | None =>
          // todo: a real implementation
          let value = Value.PrimitiveValue(NumberValue(3.0))
          ExecutionReducerReturn.f(value, execution, source)
        }
      }
    }

  | _ => raise(Exception.TODO("Evaluating a non-graph"))
  }
}
