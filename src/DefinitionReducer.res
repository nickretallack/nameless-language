let f = ({definitionID, action}: DefinitionActionRecord.t, state: AppState.t): ReactUpdate.update<
  AppAction.t,
  AppState.t,
> => {
  let definition = Belt.Map.String.getExn(state.definitions, definitionID)

  let updateDefinition = definition => ReactUpdate.Update({
    ...state,
    definitions: Belt.Map.String.set(state.definitions, definitionID, definition),
    error: NoAppError,
  })

  switch action {
  | CreateConnection({source, sink}) =>
    switch definition.implementation {
    | GraphImplementation(graphImplementation) =>
      updateDefinition({
        ...definition,
        implementation: GraphImplementation({
          ...graphImplementation,
          connections: Belt.Map.set(graphImplementation.connections, sink, source),
        }),
      })
    | _ => ReactUpdate.NoUpdate
    }

  | AddNode({node, explicitConnectionSide, connectionNib}) =>
    let nodeID = RandomIDMake.f()
    let nodeConnectionSide = {
      open ConnectionSide
      {node: NodeConnection(nodeID), nib: connectionNib}
    }
    let (source, sink) = explicitConnectionSide.isSource
      ? (explicitConnectionSide.connectionSide, nodeConnectionSide)
      : (nodeConnectionSide, explicitConnectionSide.connectionSide)
    switch definition.implementation {
    | GraphImplementation(graphImplementation) =>
      updateDefinition({
        ...definition,
        implementation: GraphImplementation({
          ...graphImplementation,
          nodes: Belt.Map.String.set(graphImplementation.nodes, nodeID, node),
          connections: Belt.Map.set(graphImplementation.connections, sink, source),
        }),
      })
    | _ => ReactUpdate.NoUpdate
    }
  | ChangeConstantValue(implementation) =>
    switch definition.implementation {
    | ConstantImplementation(_) =>
      updateDefinition({
        ...definition,
        implementation: ConstantImplementation(implementation),
      })
    | _ => raise(Exception.ShouldntHappen("ChangeConstantValue targeting non-constant"))
    }
  | ChangeLabeledType(wrappedType) =>
    switch definition.implementation {
    | LabeledTypeImplementation(_) =>
      updateDefinition({
        ...definition,
        implementation: LabeledTypeImplementation(wrappedType),
      })
    | _ => raise(Exception.ShouldntHappen("ChangeLabeledTtype targeting non-label"))
    }
  | AddUnionType(valueType) =>
    switch definition.implementation {
    | UnionTypeImplementation(typeSet) =>
      updateDefinition({
        ...definition,
        implementation: UnionTypeImplementation(Belt.Set.add(typeSet, valueType)),
      })
    | _ => raise(Exception.ShouldntHappen("AddUnionType targeting non-union"))
    }
  | RemoveUnionType(valueType) =>
    switch definition.implementation {
    | UnionTypeImplementation(typeSet) =>
      updateDefinition({
        ...definition,
        implementation: UnionTypeImplementation(Belt.Set.remove(typeSet, valueType)),
      })
    | _ => raise(Exception.ShouldntHappen("RemoveUnionType targeting non-union"))
    }
  | ChangeName(text) =>
    updateDefinition({
      ...definition,
      documentation: {
        ...definition.documentation,
        name: TranslatableSetText.f(definition.documentation.name, state.languageName, text),
      },
    })
  | ChangeDescription(text) =>
    updateDefinition({
      ...definition,
      documentation: {
        ...definition.documentation,
        name: TranslatableSetText.f(definition.documentation.description, state.languageName, text),
      },
    })
  | AddInput =>
    let nibID = RandomIDMake.f()
    updateDefinition({
      documentation: {
        ...definition.documentation,
        inputs: Belt.Map.String.set(definition.documentation.inputs, nibID, TranslatableEmpty.v),
      },
      display: {
        ...definition.display,
        inputOrdering: List.append(definition.display.inputOrdering, list{nibID}),
      },
      implementation: switch definition.implementation {
      | InterfaceImplementation(interface) =>
        InterfaceImplementation({
          ...interface,
          input: Belt.Map.String.set(interface.input, nibID, PrimitiveValueType(TextType)),
        })
      | GraphImplementation(graphImplementation) =>
        GraphImplementation({
          ...graphImplementation,
          interface: {
            ...graphImplementation.interface,
            input: Belt.Map.String.set(graphImplementation.interface.input, nibID, AnyType),
          },
        })
      | RecordTypeImplementation(typedFields) =>
        RecordTypeImplementation(
          Belt.Map.String.set(typedFields, nibID, PrimitiveValueType(TextType)),
        )
      | _ => definition.implementation
      },
    })
  | AddOutput =>
    let nibID = RandomIDMake.f()
    updateDefinition({
      documentation: {
        ...definition.documentation,
        outputs: Belt.Map.String.set(definition.documentation.outputs, nibID, TranslatableEmpty.v),
      },
      display: {
        ...definition.display,
        outputOrdering: List.append(definition.display.outputOrdering, list{nibID}),
      },
      implementation: switch definition.implementation {
      | InterfaceImplementation(interface) =>
        InterfaceImplementation({
          ...interface,
          output: Belt.Map.String.set(interface.output, nibID, PrimitiveValueType(NumberType)),
        })
      | GraphImplementation(graphImplementation) =>
        GraphImplementation({
          ...graphImplementation,
          interface: {
            ...graphImplementation.interface,
            output: Belt.Map.String.set(graphImplementation.interface.output, nibID, AnyType),
          },
        })
      | _ => definition.implementation
      },
    })
  | NibAction(nibAction) =>
    NibReducer.f(nibAction, state, definitionID, definition, updateDefinition)
  | RemoveConnection(connectionSide) =>
    updateDefinition({
      ...definition,
      implementation: switch definition.implementation {
      | GraphImplementation(graphImplementation) =>
        GraphImplementation({
          ...graphImplementation,
          connections: Belt.Map.remove(graphImplementation.connections, connectionSide),
        })
      | _ => raise(Exception.ShouldntHappen("RemoveConnection targeting non-graph"))
      },
    })
  | RemoveNodes(nodeIDs) =>
    updateDefinition({
      ...definition,
      implementation: switch definition.implementation {
      | GraphImplementation(graphImplementation) =>
        let nodeIDs = DeletionGetAffectedNodes.f(nodeIDs, graphImplementation.nodes)
        GraphImplementation({
          ...graphImplementation,
          nodes: Belt.Map.String.removeMany(
            graphImplementation.nodes,
            Belt.Set.String.toArray(nodeIDs),
          ),
          connections: Belt.Map.keep(graphImplementation.connections, (sink, source) =>
            !(
              DeletionConnectionSideInvolvesNodeIDs.f(sink, nodeIDs) ||
              DeletionConnectionSideInvolvesNodeIDs.f(source, nodeIDs)
            )
          ),
        })
      | _ => raise(Exception.ShouldntHappen("RemoveNodes targeting non-union"))
      },
    })
  | Fork =>
    let newDefinitionID = RandomIDMake.f()
    ReactUpdate.UpdateWithSideEffects(
      {
        ...state,
        definitions: Belt.Map.String.set(state.definitions, newDefinitionID, definition),
      },
      _ => {
        RescriptReactRouter.push("#" ++ newDefinitionID)
        None
      },
    )
  | ChangeNodeScope({nodeID, nodeScope}) =>
    switch definition.implementation {
    | GraphImplementation(graphImplementation) =>
      if (
        switch nodeScope {
        | GraphScope => true
        | NodeScope(scopeNodeID) =>
          NodeIsFunctionDefinition.f(
            Belt.Map.String.getExn(graphImplementation.nodes, scopeNodeID),
          ) &&
          nodeID != scopeNodeID
        }
      ) {
        let node = Belt.Map.String.getExn(graphImplementation.nodes, nodeID)
        let newNode = {...node, scope: nodeScope}
        let nodes = Belt.Map.String.set(graphImplementation.nodes, nodeID, newNode)
        if ConnectionMapCheckScopes.f(graphImplementation.connections, nodes) {
          updateDefinition({
            ...definition,
            implementation: GraphImplementation({...graphImplementation, nodes: nodes}),
          })
        } else {
          ReactUpdate.Update({
            ...state,
            error: ConnectionCrossesScopeError,
          })
        }
      } else {
        ReactUpdate.NoUpdate
      }
    | _ => ReactUpdate.NoUpdate
    }
  | EvaluateNib(explicitConnectionSide) =>
    let scopeID = RandomIDMake.f()
    ReactUpdate.Update({
      ...state,
      execution: Some({
        result: None,
        scopes: Belt.Map.String.fromArray([(scopeID, ScopeMake.f(definitionID, None))]),
        stack: list{
          {scopeID: scopeID, explicitConnectionSide: explicitConnectionSide, action: Evaluating},
        },
        reactKey: RandomIDMake.f(),
      }),
    })
  }
}
