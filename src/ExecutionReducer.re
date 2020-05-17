let f = (execution: Execution.t, definitions: DefinitionMap.t): Execution.t => {
  let frame = Belt.List.headExn(execution.stack);
  let scope = Belt.Map.String.getExn(execution.scopes, frame.scopeID);
  let definition = Belt.Map.String.getExn(definitions, scope.definitionID);
  switch (definition.implementation) {
  | GraphImplementation(graphImplementation) =>
    let source =
      ExplicitConnecttionSideGetSource.f(
        frame.explicitConnectionSide,
        graphImplementation.connections,
      );
    // check the cache to see if it's already evaluated
    switch (Belt.Map.get(scope.sourceValues, source)) {
    | Some(value) => {
        ...execution,
        stack: [
          {...frame, action: Returning(value)},
          ...Belt.List.tailExn(execution.stack),
        ],
      }
    | None =>
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
              EvaluateConstantReducer.f(
                primitiveValue,
                execution,
                frame,
                scope,
                source,
              )
            | SymbolImplementation =>
              let value =
                Value.DefinedValue({definitionID, value: SymbolValue});
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

            | _ =>
              raise(
                Exception.TODO(
                  "Evaluating a ValueNode that's not a symbol or constant",
                ),
              )
            }
          | FunctionCallNode =>
            switch (nodeDefinition.implementation) {
            | ExternalImplementation(externalImplementation) =>
              switch (source.nib) {
              | NibConnection(outputID) =>
                EvaluateExternalReducer.f(
                  execution,
                  scope,
                  frame,
                  source,
                  graphImplementation.connections,
                  externalImplementation,
                  outputID,
                )
              | _ =>
                raise(
                  Exception.ShouldntHappen(
                    "Evaluating a non-keyword nib on an external function",
                  ),
                )
              }
            | GraphImplementation(_) =>
              let (nodeScopeID, scopes) =
                switch (Belt.Map.String.get(scope.nodeScopeIDs, nodeID)) {
                | Some(nodeScopeID) => (nodeScopeID, execution.scopes)
                | None =>
                  let nodeScopeID = RandomIDMake.f();
                  (
                    nodeScopeID,
                    Belt.Map.String.set(
                      Belt.Map.String.set(
                        execution.scopes,
                        nodeScopeID,
                        ScopeMake.f(
                          definitionID,
                          Some({scopeID: frame.scopeID, nodeID}),
                        ),
                      ),
                      frame.scopeID,
                      {
                        ...scope,
                        nodeScopeIDs:
                          Belt.Map.String.set(
                            scope.nodeScopeIDs,
                            nodeID,
                            nodeScopeID,
                          ),
                      },
                    ),
                  );
                };

              {
                ...execution,
                scopes,
                stack: [
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
                ],
              };
            | _ =>
              raise(
                Exception.TODO(
                  "Evaluating a function that's not a graph or external",
                ),
              )
            }
          | ConstructorNode =>
            switch (nodeDefinition.implementation) {
            | RecordTypeImplementation(typedFields) =>
              let value =
                Value.DefinedValue({
                  definitionID,
                  value:
                    RecordValue(
                      Belt.Map.String.mapWithKey(typedFields, (nibID, _) =>
                        Value.LazyValue({
                          scopeID: frame.scopeID,
                          connectionSide: {
                            node: NodeConnection(nodeID),
                            nib: NibConnection(nibID),
                          },
                        })
                      ),
                    ),
                });
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
            | LabeledTypeImplementation(_) =>
              let value =
                Value.DefinedValue({
                  definitionID,
                  value:
                    LabeledValue(
                      LazyValue({
                        scopeID: frame.scopeID,
                        connectionSide: {
                          node: NodeConnection(nodeID),
                          nib: ValueConnection,
                        },
                      }),
                    ),
                });
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
            | _ =>
              raise(
                Exception.ShouldntHappen(
                  "Evaluating a constructor that's not a record or label",
                ),
              )
            }
          | AccessorNode =>
            switch (nodeDefinition.implementation) {
            | RecordTypeImplementation(_) =>
              switch (
                Belt.Map.get(
                  scope.sourceValues,
                  Belt.Map.getExn(
                    graphImplementation.connections,
                    {node: source.node, nib: ValueConnection},
                  ),
                )
              ) {
              | Some(value) =>
                switch (source.nib) {
                | NibConnection(nibID) =>
                  switch (value) {
                  | DefinedValue(definedValue) =>
                    switch (definedValue.value) {
                    | RecordValue(values) =>
                      switch (Belt.Map.String.getExn(values, nibID)) {
                      | LazyValue(lazyValue) =>
                        switch (
                          Belt.Map.get(
                            scope.sourceValues,
                            Belt.Map.getExn(
                              graphImplementation.connections,
                              lazyValue.connectionSide,
                            ),
                          )
                        ) {
                        | None =>
                          Js.log("None value");
                          {
                            ...execution,
                            stack: [
                              StackFrame.{
                                scopeID: lazyValue.scopeID,
                                explicitConnectionSide: {
                                  isSource: false,
                                  connectionSide: lazyValue.connectionSide,
                                },
                                action: EvaluationAction.Evaluating,
                              },
                              ...execution.stack,
                            ],
                            scopes: execution.scopes,
                          };
                        | Some(value) => {
                            ...execution,
                            stack: [
                              {...frame, action: Returning(value)},
                              ...Belt.List.tailExn(execution.stack),
                            ],
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
                  stack: [
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
                  ],
                  scopes: execution.scopes,
                }
              }
            | LabeledTypeImplementation(_) =>
              switch (
                Belt.Map.get(
                  scope.sourceValues,
                  Belt.Map.getExn(
                    graphImplementation.connections,
                    {node: source.node, nib: ValueConnection},
                  ),
                )
              ) {
              | Some(value) =>
                switch (value) {
                | DefinedValue(definedValue) =>
                  switch (definedValue.value) {
                  | LabeledValue(wrappedValue) =>
                    switch (wrappedValue) {
                    | LazyValue(lazyValue) =>
                      switch (
                        Belt.Map.get(
                          scope.sourceValues,
                          Belt.Map.getExn(
                            graphImplementation.connections,
                            lazyValue.connectionSide,
                          ),
                        )
                      ) {
                      | None =>
                        Js.log("None value");
                        {
                          ...execution,
                          stack: [
                            StackFrame.{
                              scopeID: lazyValue.scopeID,
                              explicitConnectionSide: {
                                isSource: false,
                                connectionSide: lazyValue.connectionSide,
                              },
                              action: EvaluationAction.Evaluating,
                            },
                            ...execution.stack,
                          ],
                          scopes: execution.scopes,
                        };
                      | Some(value) => {
                          ...execution,
                          stack: [
                            {...frame, action: Returning(value)},
                            ...Belt.List.tailExn(execution.stack),
                          ],
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
                  stack: [
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
                  ],
                  scopes: execution.scopes,
                }
              }
            | _ =>
              raise(
                Exception.ShouldntHappen(
                  "Evaluating an accessor that's not a record or label",
                ),
              )
            }
          | _ =>
            raise(Exception.TODO("Evaluating a new kind of defined node"))
          };
        | _ => raise(Exception.TODO("Evaluating a new kind of node"))
        };
      | GraphConnection =>
        switch (scope.parentScope) {
        | Some(parentScope) => {
            ...execution,
            stack: [
              StackFrame.{
                scopeID: parentScope.scopeID,
                explicitConnectionSide: {
                  isSource: false,
                  connectionSide: {
                    node: NodeConnection(parentScope.nodeID),
                    nib: source.nib,
                  },
                },
                action: Evaluating,
              },
              ...execution.stack,
            ],
          }
        | None =>
          // todo: a real implementation
          let value = Value.PrimitiveValue(NumberValue(3.0));
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
        }
      }
    };

  | _ => raise(Exception.TODO("Evaluating a non-graph"))
  };
};
