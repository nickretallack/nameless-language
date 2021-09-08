type category =
  | NumberCategory
  | AllCategory
type state = {
  category: option<category>,
  definitionID: option<DefinitionID.t>,
  definedNodeKind: option<DefinedNodeKind.t>,
}
type addNode = {
  node: Node.t,
  connectionNib: ConnectionNib.t,
}
type action =
  | SelectCategory(category)
  | SelectDefinition(DefinitionID.t)
  | AddValue
  | SetDefinedNodeKind(DefinedNodeKind.t)
let isNumberConstant = (definition: Definition.t): bool =>
  switch definition.implementation {
  | ConstantImplementation(NumberValue(_)) => true
  | _ => false
  }
let getScope = (nib: ExplicitConnectionSide.t, nodes: NodeMap.t) =>
  switch nib.connectionSide.node {
  | GraphConnection => NodeScope.GraphScope
  | NodeConnection(nodeID) =>
    let node = Belt.Map.String.getExn(nodes, nodeID)
    NodeIsFunctionDefinition.f(node) && !ConnectionNibIsValue.f(nib.connectionSide.nib)
      ? NodeScope(nodeID)
      : node.scope
  }
let maybeNameless = (string: string) => String.length(string) == 0 ? "(nameless)" : string
let canConnectToNib = (definition: Definition.t, isSource: bool) =>
  !isSource ||
  (Belt.List.length(definition.display.inputOrdering) != 0 ||
  switch definition.implementation {
  | ConstantImplementation(_) => false
  | LabeledTypeImplementation(_) => true
  | _ => true
  })

@react.component
let make = (
  ~graphDefinitionID: DefinitionID.t,
  ~definitions: DefinitionMap.t,
  ~nodes: NodeMap.t,
  ~selection: Selection.t,
  ~emit: DefinitionAction.t => unit,
  ~languageName: LanguageName.t,
) => {
  switch selection {
  | SelectedNib(explicitConnectionSide) =>
    let nib = explicitConnectionSide
    let (state, send) = ReactUpdate.useReducer(
      {category: None, definitionID: None, definedNodeKind: None},
      (action: action, state: state) =>
        switch action {
        | SelectCategory(category) =>
          ReactUpdate.Update({
            category: Some(category),
            definitionID: None,
            definedNodeKind: None,
          })
        | SetDefinedNodeKind(definedNodeKind) =>
          ReactUpdate.Update({
            ...state,
            definedNodeKind: Some(definedNodeKind),
          })
        | SelectDefinition(definitionID) =>
          switch Belt.Map.String.getExn(definitions, definitionID).implementation {
          | ConstantImplementation(_) =>
            if nib.isSource {
              ReactUpdate.NoUpdate
            } else {
              ReactUpdate.SideEffects(
                _ => {
                  emit(
                    AddNode({
                      node: {
                        kind: DefinedNode({kind: ValueNode, definitionID: definitionID}),
                        scope: getScope(nib, nodes),
                      },
                      explicitConnectionSide: nib,
                      connectionNib: ValueConnection,
                    }),
                  )
                  RescriptReactRouter.push(`#${graphDefinitionID}`)
                  None
                },
              )
            }
          | _ =>
            ReactUpdate.Update({
              ...state,
              definitionID: Some(definitionID),
              definedNodeKind: None,
            })
          }
        | AddValue =>
          if nib.isSource && state.definedNodeKind != Some(ConstructorNode) {
            ReactUpdate.NoUpdate
          } else {
            ReactUpdate.SideEffects(
              _ => {
                emit(
                  AddNode({
                    node: {
                      kind: DefinedNode({
                        kind: ValueNode,
                        definitionID: switch state.definitionID {
                        | None => raise(Not_found)
                        | Some(definitionID) => definitionID
                        },
                      }),
                      scope: getScope(nib, nodes),
                    },
                    explicitConnectionSide: nib,
                    connectionNib: ValueConnection,
                  }),
                )
                RescriptReactRouter.push(`#${graphDefinitionID}`)
                None
              },
            )
          }
        },
    )

    let renderCategory = (name: string, category: category) =>
      <a
        onClick={_event => send(SelectCategory(category))}
        className={state.category == Some(category) ? "selected" : ""}>
        {React.string(name)}
      </a>
    let nodeSelector = (filterFunction: Definition.t => bool) =>
      <div className="type-selector-choices">
        <h3> {React.string("Definitions")} </h3>
        {React.array(
          Belt.Array.map(
            Belt.Map.String.toArray(
              Belt.Map.String.keep(definitions, (
                _definitionID: DefinitionID.t,
                definition: Definition.t,
              ) => filterFunction(definition) && canConnectToNib(definition, nib.isSource)),
            ),
            ((definitionID: DefinitionID.t, definition: Definition.t)) =>
              <a
                key=definitionID
                className={state.definitionID == Some(definitionID) ? "selected" : ""}
                onClick={_event => send(SelectDefinition(definitionID))}>
                {React.string(DefinitionGetDisplayName.f(definition, languageName))}
              </a>,
          ),
        )}
      </div>
    let nodeTypeLink = (kind, name) =>
      <a
        className={state.definedNodeKind == Some(kind) ? "selected" : ""}
        onClick={_event => send(SetDefinedNodeKind(kind))}>
        {React.string(name)}
      </a>
    <>
      <div className="type-selector-menu">
        <div className="type-selector-categories">
          <h3> {React.string("Category")} </h3> {renderCategory("Defined", AllCategory)}
        </div>
        {switch state.category {
        | None => React.null
        | Some(category) =>
          switch category {
          | NumberCategory => nodeSelector(isNumberConstant)
          | AllCategory => nodeSelector(_ => true)
          }
        }}
        {switch state.definitionID {
        | None => React.null
        | Some(definitionID) =>
          let definition = Belt.Map.String.getExn(definitions, definitionID)
          <>
            <div>
              <h3> {React.string("Usage")} </h3>
              {switch definition.implementation {
              | GraphImplementation(_)
              | ExternalImplementation(_) => <>
                  {nodeTypeLink(FunctionCallNode, "call")}
                  {nib.isSource
                    ? React.null
                    : <a onClick={_event => send(AddValue)}> {React.string("value")} </a>}
                </>
              | InterfaceImplementation(_) => <>
                  {nodeTypeLink(FunctionPointerCallNode, "function pointer call")}
                  {nib.isSource
                    ? React.null
                    : <a
                        onClick={_event => {
                          emit(
                            AddNode({
                              node: {
                                scope: getScope(nib, nodes),
                                kind: DefinedNode({
                                  kind: FunctionDefinitionNode,
                                  definitionID: definitionID,
                                }),
                              },
                              explicitConnectionSide: nib,
                              connectionNib: ValueConnection,
                            }),
                          )
                          RescriptReactRouter.push(`#${graphDefinitionID}`)
                        }}>
                        {React.string("inline function")}
                      </a>}
                </>
              | RecordTypeImplementation(_) =>
                nib.isSource
                  ? <>
                      {nodeTypeLink(ConstructorNode, "constructor")}
                      <a
                        onClick={_event => {
                          emit(
                            AddNode({
                              node: {
                                scope: getScope(nib, nodes),
                                kind: DefinedNode({
                                  kind: AccessorNode,
                                  definitionID: definitionID,
                                }),
                              },
                              explicitConnectionSide: nib,
                              connectionNib: ValueConnection,
                            }),
                          )
                          RescriptReactRouter.push(`#${graphDefinitionID}`)
                        }}>
                        {React.string("accessor")}
                      </a>
                    </>
                  : <>
                      <a
                        onClick={_event => {
                          emit(
                            AddNode({
                              node: {
                                scope: getScope(nib, nodes),
                                kind: DefinedNode({
                                  kind: ConstructorNode,
                                  definitionID: definitionID,
                                }),
                              },
                              explicitConnectionSide: nib,
                              connectionNib: ValueConnection,
                            }),
                          )
                          RescriptReactRouter.push(`#${graphDefinitionID}`)
                        }}>
                        {React.string("constructor")}
                      </a>
                      {nodeTypeLink(AccessorNode, "accessor")}
                    </>
              | SymbolImplementation => <>
                  <a
                    onClick={_event => {
                      emit(
                        AddNode({
                          node: {
                            scope: getScope(nib, nodes),
                            kind: DefinedNode({kind: ValueNode, definitionID: definitionID}),
                          },
                          explicitConnectionSide: nib,
                          connectionNib: ValueConnection,
                        }),
                      )
                      RescriptReactRouter.push(`#${graphDefinitionID}`)
                    }}>
                    {React.string("value")}
                  </a>
                </>
              | LabeledTypeImplementation(_) => <>
                  {!nib.isSource
                    ? <a
                        onClick={_event => {
                          emit(
                            AddNode({
                              node: {
                                scope: getScope(nib, nodes),
                                kind: DefinedNode({
                                  kind: ConstructorNode,
                                  definitionID: definitionID,
                                }),
                              },
                              explicitConnectionSide: nib,
                              connectionNib: ValueConnection,
                            }),
                          )
                          RescriptReactRouter.push(`#${graphDefinitionID}`)
                        }}>
                        {React.string("constructor")}
                      </a>
                    : React.null}
                  <a
                    onClick={_event => {
                      emit(
                        AddNode({
                          node: {
                            scope: getScope(nib, nodes),
                            kind: DefinedNode({kind: AccessorNode, definitionID: definitionID}),
                          },
                          explicitConnectionSide: nib,
                          connectionNib: ValueConnection,
                        }),
                      )
                      RescriptReactRouter.push(`#${graphDefinitionID}`)
                    }}>
                    {React.string("accessor")}
                  </a>
                </>
              | UnionTypeImplementation(_) => React.string("Can't create nodes for union types")
              | _ => <> {React.string("TODO")} </>
              }}
            </div>
            {switch state.definedNodeKind {
            | None => React.null
            | Some(definedNodeKind) =>
              let display = NodeGetDisplayNibs.f(
                {
                  scope: getScope(nib, nodes),
                  kind: DefinedNode({kind: definedNodeKind, definitionID: definitionID}),
                },
                definitions,
                languageName,
              )
              <div>
                <h3> {React.string(nib.isSource ? "Input" : "Output")} </h3>
                {React.array(
                  Belt.List.toArray(
                    Belt.List.map(nib.isSource ? display.inputs : display.outputs, (
                      displayNib: DisplayNib.t,
                    ) =>
                      <a
                        key={ConnectionNibKey.f(displayNib.nib)}
                        onClick={_event => {
                          emit(
                            AddNode({
                              node: {
                                scope: getScope(nib, nodes),
                                kind: DefinedNode({
                                  kind: definedNodeKind,
                                  definitionID: definitionID,
                                }),
                              },
                              explicitConnectionSide: nib,
                              connectionNib: displayNib.nib,
                            }),
                          )
                          RescriptReactRouter.push(`#${graphDefinitionID}`)
                        }}>
                        {React.string(maybeNameless(displayNib.name))}
                      </a>
                    ),
                  ),
                )}
              </div>
            }}
          </>
        }}
      </div>
    </>

  | _ => React.string("TODO")
  }
}
