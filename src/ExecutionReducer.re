let f = (execution: Execution.t, definitions: DefinitionMap.t): Execution.t => {
  let frame = Belt.List.headExn(execution.stack);
  let scope = Belt.Map.String.getExn(execution.scopes, frame.scopeID);
  let definition = Belt.Map.String.getExn(definitions, scope.definitionID);
  switch (definition.implementation) {
  | GraphImplementation(graphImplementation) =>
    let source =
      frame.explicitConnectionSide.isSource
        ? frame.explicitConnectionSide.connectionSide
        : Belt.Map.getExn(
            graphImplementation.connections,
            frame.explicitConnectionSide.connectionSide,
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
            | _ => raise(Not_found)
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
              | _ => raise(Not_found)
              }
            | GraphImplementation(_graphImplementation) =>
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
                        ScopeMake.f(definitionID),
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
            | _ => raise(Not_found) // todo
            }
          | ConstructorNode =>
            switch (nodeDefinition.implementation) {
            | RecordTypeImplementation(typedFields) =>
              let value =
                Value.DefinedValue({
                  definitionID,
                  values:
                    Belt.Map.String.mapWithKey(typedFields, (nibID, _) =>
                      Value.LazyValue({
                        scopeID: frame.scopeID,
                        connectionSide: {
                          node: NodeConnection(nodeID),
                          nib: NibConnection(nibID),
                        },
                      })
                    ),
                });
              Js.log(source);
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
            | _ => raise(Not_found)
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
                    switch (
                      Belt.Map.String.getExn(definedValue.values, nibID)
                    ) {
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
            | _ => raise(Not_found)
            }
          | _ => raise(Not_found) // todo
          };
        | _ => raise(Not_found) // todo
        };
      | GraphConnection =>
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
                sourceValues: Belt.Map.set(scope.sourceValues, source, value),
              },
            ),
        };
      }
    };

  | _ => raise(Not_found) // todo
  };
};
