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
  | Publish =>
    // TODO: topologically sort the dependency graph
    let dependencyMap = ImplementationGetDependencyMap.f(definitionID, state.definitions)
    Js.log2(
      "Dependency map:",
      Belt.Array.map(Belt.Map.String.toArray(dependencyMap), ((key, value)) => (
        key,
        Belt.Set.String.toArray(value),
      )),
    )

    let components = DependencyMapTarjan.f(dependencyMap)
    Js.log2("Components:", components)

    let publishing = Belt.Array.reduceReverse(components, Belt.Map.String.empty, (
      dependencies,
      component,
    ) => {
      if Belt.Array.length(component) == 1 {
        let definitionID = component[0]
        let definition = Belt.Map.String.getExn(state.definitions, definitionID)
        Belt.Map.String.set(
          dependencies,
          definitionID,
          DefinitionToPublishingDependency.f(definitionID, definition, dependencies),
        )
      } else {
        // serialize the cycle members once to determine their sort order
        let unorderedMutualRecursionStubs = Belt.Array.map(component, definitionID => {
          let definition = Belt.Map.String.getExn(state.definitions, definitionID)
          (
            definitionID,
            {
              PublishingDependency.kind: UnorderedMutualRecursion,
              inputOrdering: definition.display.inputOrdering,
              outputOrdering: definition.display.outputOrdering,
            },
          )
        })
        let temporaryDependencies = Belt.Map.String.mergeMany(
          dependencies,
          unorderedMutualRecursionStubs,
        )

        // sort by preliminary canonical string
        let orderedComponent = Belt.List.toArray(
          ListSortBy.f(Belt.List.fromArray(component), definitionID => {
            let definition = Belt.Map.String.getExn(state.definitions, definitionID)
            DefinitionToCanonicalString.f(definitionID, definition, temporaryDependencies)
          }),
        )

        // calculate content IDs again now that we have an ordering
        let orderedMutualRecursionStubs = Belt.Array.mapWithIndex(component, (
          index,
          definitionID,
        ) => {
          let definition = Belt.Map.String.getExn(state.definitions, definitionID)
          (
            definitionID,
            {
              PublishingDependency.kind: OrderedMutualRecursion(index),
              inputOrdering: definition.display.inputOrdering,
              outputOrdering: definition.display.outputOrdering,
            },
          )
        })
        let orderedDependencies = Belt.Map.String.mergeMany(
          dependencies,
          orderedMutualRecursionStubs,
        )

        let canonicalStrings = Belt.Array.map(orderedComponent, definitionID => {
          let definition = Belt.Map.String.getExn(state.definitions, definitionID)
          DefinitionToCanonicalString.f(definitionID, definition, orderedDependencies)
        })

        let canonicalString = Json.stringify(
          Json.Encode.array(Json.Encode.string, canonicalStrings),
        )

        let contentID = ReScriptHash.Sha256.make(canonicalString)
        let hashedContent = {
          HashedContent.contentID: contentID,
          canonicalString: canonicalString,
        }

        Belt.Array.reduceWithIndex(component, dependencies, (dependencies, definitionID, index) => {
          // Each definition in the cycle comes up with ids for its mutuals
          let definition = Belt.Map.String.getExn(state.definitions, definitionID)
          Belt.Map.String.set(
            dependencies,
            definitionID,
            {
              kind: FinalMutualRecursion({position: index, content: hashedContent}),
              inputOrdering: definition.display.inputOrdering,
              outputOrdering: definition.display.outputOrdering,
            },
          )
        })
      }
    })
    Js.log2("Publishing:", Belt.Map.String.toArray(publishing))

    ReactUpdate.NoUpdate
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
        inputOrdering: Belt.Array.concat(definition.display.inputOrdering, [nibID]),
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
        outputOrdering: Belt.Array.concat(definition.display.outputOrdering, [nibID]),
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
  | NewFunctionFromNodes(nodeIDs) =>
    switch definition.implementation {
    | GraphImplementation(graphImplementation) =>
      let (innerNodes, outerNodes, scopes) = GraphNodesGetScopes.f(graphImplementation, nodeIDs)
      if Belt.Set.size(scopes) != 1 {
        Js.log("Tried to join nodes from different scopes.") // TODO: make the button not show up in this case
        ReactUpdate.NoUpdate
      } else {
        let scope = Belt.Set.toArray(scopes)[0]
        let newNodeID = RandomIDMake.f()
        let innerDefinitionID = RandomIDMake.f()
        let outerNodes = Belt.Map.String.set(
          outerNodes,
          newNodeID,
          {
            scope: scope,
            kind: DefinedNode({
              kind: FunctionCallNode,
              definitionID: innerDefinitionID,
            }),
          },
        )

        let (
          inboundConnections,
          outboundConnections,
          innerConnections,
        ) = Belt.Map.reduce(
          graphImplementation.connections,
          (
            ConnectionMapInverted.fromArray([]),
            ConnectionMapInverted.fromArray([]),
            ConnectionMap.fromArray([]),
          ),
          ((inbound, outbound, contained), sink: ConnectionSide.t, source: ConnectionSide.t) => {
            let isInside = (connectionNode: ConnectionNode.t): bool =>
              switch connectionNode {
              | GraphConnection => false
              | NodeConnection(nodeID) => Belt.Map.String.has(innerNodes, nodeID)
              }
            let addToInverse = (
              connections: ConnectionMapInverted.t,
              source: ConnectionSide.t,
              sink: ConnectionSide.t,
            ): ConnectionMapInverted.t => {
              Belt.Map.update(connections, source, maybeSinks =>
                switch maybeSinks {
                | None => Some(ConnectionSideSet.fromArray([sink]))
                | Some(sinks) => Some(Belt.Set.add(sinks, sink))
                }
              )
            }

            let sinkInside = isInside(sink.node)
            let sourceInside = isInside(source.node)
            if sinkInside && sourceInside {
              (inbound, outbound, Belt.Map.set(contained, sink, source))
            } else if sourceInside {
              (inbound, addToInverse(outbound, source, sink), contained)
            } else if sinkInside {
              (addToInverse(inbound, source, sink), outbound, contained)
            } else {
              (inbound, outbound, contained)
            }
          },
        )

        let outputNibs = Belt.Map.String.fromArray([])
        let inputNibs = Belt.Map.String.fromArray([])
        let outerConnections = Belt.Map.keep(graphImplementation.connections, (sink, _source) =>
          !Belt.Map.has(innerConnections, sink)
        )

        let (outputNibs, outerConnections, innerConnections) = Belt.Map.reduce(
          outboundConnections,
          (outputNibs, outerConnections, innerConnections),
          ((nibs, outerConnections, innerConnections), source, sinks) => {
            let nibID = RandomIDMake.f()
            let nibConnection = ConnectionNib.NibConnection(nibID)
            let outerConnectionSide = {
              ConnectionSide.node: NodeConnection(newNodeID),
              nib: nibConnection,
            }
            let innerConnectionSide = {ConnectionSide.node: GraphConnection, nib: nibConnection}

            let nibs = Belt.Map.String.set(nibs, nibID, ValueType.AnyType)
            // old connections get overwritten so we don't need to manually remove them
            let outerConnections = Belt.Set.reduce(sinks, outerConnections, (
              outerConnections,
              sink,
            ) => {
              Belt.Map.set(outerConnections, sink, outerConnectionSide)
            })
            let innerConnections = Belt.Map.set(innerConnections, innerConnectionSide, source)

            (nibs, outerConnections, innerConnections)
          },
        )

        let (inputNibs, outerConnections, innerConnections) = Belt.Map.reduce(
          inboundConnections,
          (inputNibs, outerConnections, innerConnections),
          ((nibs, outerConnections, innerConnections), source, sinks) => {
            let nibID = RandomIDMake.f()
            let nibConnection = ConnectionNib.NibConnection(nibID)
            let outerConnectionSide = {
              ConnectionSide.node: NodeConnection(newNodeID),
              nib: nibConnection,
            }
            let innerConnectionSide = {ConnectionSide.node: GraphConnection, nib: nibConnection}

            let nibs = Belt.Map.String.set(nibs, nibID, ValueType.AnyType)
            let innerConnections = Belt.Set.reduce(sinks, innerConnections, (
              innerConnections,
              sink,
            ) => {
              Belt.Map.set(innerConnections, sink, innerConnectionSide)
            })
            let outerConnections = Belt.Map.set(outerConnections, outerConnectionSide, source)
            // remove old connections
            let outerConnections = Belt.Set.reduce(sinks, outerConnections, (
              outerConnections,
              sink,
            ) => Belt.Map.remove(outerConnections, sink))

            (nibs, outerConnections, innerConnections)
          },
        )

        let innerGraph = {
          Definition.implementation: GraphImplementation({
            nodes: innerNodes,
            connections: innerConnections,
            interface: {
              input: inputNibs,
              output: outputNibs,
            },
          }),
          display: {
            inputOrdering: Belt.Map.String.keysToArray(inputNibs),
            outputOrdering: Belt.Map.String.keysToArray(outputNibs),
          },
          documentation: {
            name: TranslatableEmpty.v,
            description: TranslatableEmpty.v,
            inputs: Belt.Map.String.map(inputNibs, _ => TranslatableEmpty.v),
            outputs: Belt.Map.String.map(outputNibs, _ => TranslatableEmpty.v),
          },
        }

        let outerGraph = {
          ...definition,
          implementation: GraphImplementation({
            ...graphImplementation,
            nodes: outerNodes,
            connections: outerConnections,
          }),
        }

        let newDefinitions = Belt.Map.String.set(state.definitions, innerDefinitionID, innerGraph)
        let newDefinitions = Belt.Map.String.set(newDefinitions, definitionID, outerGraph)

        ReactUpdate.Update({
          ...state,
          definitions: newDefinitions,
        })
      }
    | _ => ReactUpdate.NoUpdate
    }

  | EvaluateNib({explicitConnectionSide, debug}) =>
    let scopeID = RandomIDMake.f()
    ReactUpdate.UpdateWithSideEffects(
      {
        ...state,
        execution: Some({
          references: Belt.Map.String.empty,
          result: Some(
            Value.LazyValue({
              scopeID: scopeID,
              explicitConnectionSide: explicitConnectionSide,
            }),
          ),
          scopes: Belt.Map.String.fromArray([
            (scopeID, ScopeMake.f(definitionID, None, GraphScope)),
          ]),
          stack: list{
            {scopeID: scopeID, explicitConnectionSide: explicitConnectionSide, action: Evaluating},
          },
          reactKey: RandomIDMake.f(),
          debug: debug,
        }),
      },
      ({send}) => {
        if !debug {
          send(Step)
        }
        None
      },
    )
  }
}
