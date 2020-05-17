type category =
  | NumberCategory
  | AllCategory;
type state = {
  category: option(category),
  definitionID: option(DefinitionID.t),
  definedNodeKind: option(DefinedNodeKind.t),
};
type addNode = {
  node: Node.t,
  connectionNib: ConnectionNib.t,
};
type action =
  | SelectCategory(category)
  | SelectDefinition(DefinitionID.t)
  | AddValue
  | SetDefinedNodeKind(DefinedNodeKind.t);
let isNumberConstant = (definition: Definition.t): bool =>
  switch (definition.implementation) {
  | ConstantImplementation(NumberValue(_)) => true
  | _ => false
  };
let getScope = (nib: ExplicitConnectionSide.t, nodes: NodeMap.t) =>
  switch (nib.connectionSide.node) {
  | GraphConnection => NodeScope.GraphScope
  | NodeConnection(nodeID) =>
    let node = Belt.Map.String.getExn(nodes, nodeID);
    NodeIsFunctionDefinition.f(node)
    && !ConnectionNibIsValue.f(nib.connectionSide.nib)
      ? NodeScope(nodeID) : node.scope;
  };
let maybeNameless = (string: string) =>
  String.length(string) == 0 ? "(nameless)" : string;
let canConnectToNib = (definition: Definition.t, isSource: bool) =>
  !isSource
  || Belt.List.length(definition.display.inputOrdering) != 0
  || (
    switch (definition.implementation) {
    | ConstantImplementation(_) => false
    | LabeledTypeImplementation(wrappedType) =>
      Belt.Option.isSome(wrappedType)
    | _ => true
    }
  );

[@react.component]
let make =
    (
      ~definitions: DefinitionMap.t,
      ~nodes: NodeMap.t,
      ~nib: ExplicitConnectionSide.t,
      ~emit: DefinitionAction.t => unit,
      ~languageName: LanguageName.t,
    ) => {
  let (state, send) =
    ReactUpdate.useReducer(
      {category: None, definitionID: None, definedNodeKind: None},
      (action: action, state: state) =>
      switch (action) {
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
        switch (
          Belt.Map.String.getExn(definitions, definitionID).implementation
        ) {
        | ConstantImplementation(_) =>
          if (nib.isSource) {
            ReactUpdate.NoUpdate;
          } else {
            ReactUpdate.SideEffects(
              _ => {
                emit(
                  AddNode({
                    node: {
                      kind: DefinedNode({kind: ValueNode, definitionID}),
                      scope: getScope(nib, nodes),
                    },
                    explicitConnectionSide: nib,
                    connectionNib: ValueConnection,
                  }),
                );
                None;
              },
            );
          }
        | _ =>
          ReactUpdate.Update({
            ...state,
            definitionID: Some(definitionID),
            definedNodeKind: None,
          })
        }
      | AddValue =>
        if (nib.isSource && state.definedNodeKind != Some(ConstructorNode)) {
          ReactUpdate.NoUpdate;
        } else {
          ReactUpdate.SideEffects(
            _ => {
              emit(
                AddNode({
                  node: {
                    kind:
                      DefinedNode({
                        kind: ValueNode,
                        definitionID:
                          switch (state.definitionID) {
                          | None => raise(Not_found)
                          | Some(definitionID) => definitionID
                          },
                      }),
                    scope: getScope(nib, nodes),
                  },
                  explicitConnectionSide: nib,
                  connectionNib: ValueConnection,
                }),
              );
              None;
            },
          );
        }
      }
    );

  let scope = getScope(nib, nodes);
  let renderCategory = (name: string, category: category) =>
    <a
      onClick={_event => send(SelectCategory(category))}
      className={state.category == Some(category) ? "selected" : ""}>
      {ReasonReact.string(name)}
    </a>;
  let nodeSelector = (filterFunction: Definition.t => bool) =>
    <div className="type-selector-choices">
      <h3> {ReasonReact.string("Definitions")} </h3>
      {ReasonReact.array(
         Belt.Array.map(
           Belt.Map.String.toArray(
             Belt.Map.String.keep(
               definitions,
               (_definitionID: DefinitionID.t, definition: Definition.t) =>
               filterFunction(definition)
               && canConnectToNib(definition, nib.isSource)
             ),
           ),
           ((definitionID: DefinitionID.t, definition: Definition.t)) =>
           <a
             key=definitionID
             className={
               state.definitionID == Some(definitionID) ? "selected" : ""
             }
             onClick={_event => send(SelectDefinition(definitionID))}>
             {ReasonReact.string(
                DefinitionGetDisplayName.f(definition, languageName),
              )}
           </a>
         ),
       )}
    </div>;
  let nodeTypeLink = (kind, name) =>
    <a
      className={state.definedNodeKind == Some(kind) ? "selected" : ""}
      onClick={_event => send(SetDefinedNodeKind(kind))}>
      {ReasonReact.string(name)}
    </a>;
  <>
    <h2> {ReasonReact.string("Create a node")} </h2>
    <div className="type-selector-menu">
      <div className="type-selector-categories">
        <h3> {ReasonReact.string("Category")} </h3>
        {renderCategory("Defined", AllCategory)}
        {nib.isSource
           ? ReasonReact.null
           : <>
               <a
                 onClick={_event =>
                   emit(
                     AddNode({
                       node: {
                         kind: ReferenceNode,
                         scope,
                       },
                       explicitConnectionSide: nib,
                       connectionNib: ValueConnection,
                     }),
                   )
                 }>
                 {ReasonReact.string("Reference")}
               </a>
             </>}
      </div>
      {switch (state.category) {
       | None => ReasonReact.null
       | Some(category) =>
         switch (category) {
         | NumberCategory => nodeSelector(isNumberConstant)
         | AllCategory => nodeSelector(_ => true)
         }
       }}
      {switch (state.definitionID) {
       | None => ReasonReact.null
       | Some(definitionID) =>
         let definition = Belt.Map.String.getExn(definitions, definitionID);
         <>
           <div>
             <h3> {ReasonReact.string("Usage")} </h3>
             {switch (definition.implementation) {
              | GraphImplementation(_)
              | ExternalImplementation(_) =>
                <>
                  {nodeTypeLink(FunctionCallNode, "call")}
                  {nib.isSource
                     ? ReasonReact.null
                     : <a onClick={_event => send(AddValue)}>
                         {ReasonReact.string("value")}
                       </a>}
                </>
              | InterfaceImplementation(_) =>
                <>
                  {nodeTypeLink(
                     FunctionPointerCallNode,
                     "function pointer call",
                   )}
                  {nib.isSource
                     ? ReasonReact.null
                     : <a
                         onClick={_event =>
                           emit(
                             AddNode({
                               node: {
                                 scope: getScope(nib, nodes),
                                 kind:
                                   DefinedNode({
                                     kind: FunctionDefinitionNode,
                                     definitionID,
                                   }),
                               },
                               explicitConnectionSide: nib,
                               connectionNib: ValueConnection,
                             }),
                           )
                         }>
                         {ReasonReact.string("inline function")}
                       </a>}
                </>
              | RecordTypeImplementation(_) =>
                nib.isSource
                  ? <>
                      {nodeTypeLink(ConstructorNode, "constructor")}
                      <a
                        onClick={_event =>
                          emit(
                            AddNode({
                              node: {
                                scope: getScope(nib, nodes),
                                kind:
                                  DefinedNode({
                                    kind: AccessorNode,
                                    definitionID,
                                  }),
                              },
                              explicitConnectionSide: nib,
                              connectionNib: ValueConnection,
                            }),
                          )
                        }>
                        {ReasonReact.string("accessor")}
                      </a>
                    </>
                  : <>
                      <a
                        onClick={_event =>
                          emit(
                            AddNode({
                              node: {
                                scope: getScope(nib, nodes),
                                kind:
                                  DefinedNode({
                                    kind: ConstructorNode,
                                    definitionID,
                                  }),
                              },
                              explicitConnectionSide: nib,
                              connectionNib: ValueConnection,
                            }),
                          )
                        }>
                        {ReasonReact.string("constructor")}
                      </a>
                      {nodeTypeLink(AccessorNode, "accessor")}
                    </>

              | LabeledTypeImplementation(wrappedType) =>
                <>
                  {!nib.isSource || Belt.Option.isSome(wrappedType)
                     ? <>
                         <a
                           onClick={_event =>
                             emit(
                               AddNode({
                                 node: {
                                   scope: getScope(nib, nodes),
                                   kind:
                                     DefinedNode({
                                       kind: ConstructorNode,
                                       definitionID,
                                     }),
                                 },
                                 explicitConnectionSide: nib,
                                 connectionNib: ValueConnection,
                               }),
                             )
                           }>
                           {ReasonReact.string("constructor")}
                         </a>
                       </>
                     : ReasonReact.null}
                  {Belt.Option.isSome(wrappedType)
                     ? <a
                         onClick={_event =>
                           emit(
                             AddNode({
                               node: {
                                 scope: getScope(nib, nodes),
                                 kind:
                                   DefinedNode({
                                     kind: AccessorNode,
                                     definitionID,
                                   }),
                               },
                               explicitConnectionSide: nib,
                               connectionNib: ValueConnection,
                             }),
                           )
                         }>
                         {ReasonReact.string("accessor")}
                       </a>
                     : ReasonReact.null}
                </>
              | UnionTypeImplementation(_) =>
                ReasonReact.string("Can't create nodes for union types")
              | _ => <> {ReasonReact.string("TODO")} </>
              }}
           </div>
           {switch (state.definedNodeKind) {
            | None => ReasonReact.null
            | Some(definedNodeKind) =>
              let display =
                NodeGetDisplayNibs.f(
                  {
                    scope: getScope(nib, nodes),
                    kind: DefinedNode({kind: definedNodeKind, definitionID}),
                  },
                  definitions,
                  languageName,
                );
              <div>
                <h3>
                  {ReasonReact.string(nib.isSource ? "Input" : "Output")}
                </h3>
                {ReasonReact.array(
                   Belt.List.toArray(
                     Belt.List.map(
                       nib.isSource ? display.inputs : display.outputs,
                       (displayNib: DisplayNib.t) =>
                       <a
                         key={ConnectionNibKey.f(displayNib.nib)}
                         onClick={_event =>
                           emit(
                             AddNode({
                               node: {
                                 scope: getScope(nib, nodes),
                                 kind:
                                   DefinedNode({
                                     kind: definedNodeKind,
                                     definitionID,
                                   }),
                               },
                               explicitConnectionSide: nib,
                               connectionNib: displayNib.nib,
                             }),
                           )
                         }>
                         {ReasonReact.string(maybeNameless(displayNib.name))}
                       </a>
                     ),
                   ),
                 )}
              </div>;
            }}
         </>;
       }}
    </div>
  </>;
};
