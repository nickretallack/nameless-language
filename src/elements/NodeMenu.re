[%%debugger.chrome];
open Definition;
open! AppActions;

/* type category =
   | ReferenceCategory
   | ListCategory
   | ConstantCategory
   | FunctionCategory
   | FunctionInterfaceCategory
   | RecordCategory; */

type category =
  | NumberCategory
  | AllCategory;

type state = {
  category: option(category),
  definitionID: option(definitionID),
  definedNodeKind: option(definedNodeKind),
};

type addNode = {
  node,
  connectionNib,
};

type action =
  | SelectCategory(category)
  | SelectDefinition(definitionID)
  | AddValue
  | SetDefinedNodeKind(definedNodeKind);

let isNumberConstant = (definition: definition): bool =>
  switch (definition.implementation) {
  | ConstantImplementation(NumberValue(_)) => true
  | _ => false
  };

let getScope = (nib: explicitConnectionSide, nodes: nodes) =>
  switch (nib.connectionSide.node) {
  | GraphConnection => GraphScope
  | NodeConnection(nodeID) =>
    let node = Belt.Map.String.getExn(nodes, nodeID);
    isFunctionDefinitionNode(node) && !isValueNib(nib.connectionSide.nib) ?
      NodeScope(nodeID) : node.scope;
  };

let maybeNameless = (string: string) =>
  String.length(string) == 0 ? "(nameless)" : string;

let canConnectToNib = (definition: definition, isSource: bool) =>
  !isSource
  || Belt.List.length(definition.display.inputOrdering) != 0
  && (
    switch (definition.implementation) {
    | ConstantImplementation(_) => false
    | _ => true
    }
  );

let component = ReasonReact.reducerComponent("NodeMenu");
let make =
    (
      ~definitions: definitions,
      ~nodes: nodes,
      ~nib: explicitConnectionSide,
      ~emit: definitionAction => unit,
      _children,
    ) => {
  ...component,
  initialState: () => {
    category: None,
    definitionID: None,
    definedNodeKind: None,
  },
  reducer: (action: action, state: state) =>
    switch (action) {
    | SelectCategory(category) =>
      ReasonReact.Update({
        category: Some(category),
        definitionID: None,
        definedNodeKind: None,
      })
    | SetDefinedNodeKind(definedNodeKind) =>
      ReasonReact.Update({...state, definedNodeKind: Some(definedNodeKind)})
    | SelectDefinition(definitionID) =>
      switch (
        Belt.Map.String.getExn(definitions, definitionID).implementation
      ) {
      | ConstantImplementation(_) =>
        if (nib.isSource) {
          ReasonReact.NoUpdate;
        } else {
          ReasonReact.SideEffects(
            _ =>
              emit(
                AddNode({
                  node: {
                    kind: DefinedNode({kind: ValueNode, definitionID}),
                    scope: getScope(nib, nodes),
                  },
                  explicitConnectionSide: nib,
                  connectionNib: ValueConnection,
                }),
              ),
          );
        }
      | _ =>
        ReasonReact.Update({
          ...state,
          definitionID: Some(definitionID),
          definedNodeKind: None,
        })
      }
    | AddValue =>
      if (nib.isSource && state.definedNodeKind != Some(ConstructorNode)) {
        ReasonReact.NoUpdate;
      } else {
        ReasonReact.SideEffects(
          _ =>
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
            ),
        );
      }
    },
  render: self => {
    let scope = getScope(nib, nodes);

    let renderCategory = (name: string, category: category) =>
      <a
        onClick={_event => self.send(SelectCategory(category))}
        className={self.state.category == Some(category) ? "selected" : ""}>
        {ReasonReact.string(name)}
      </a>;

    let nodeSelector = (filterFunction: definition => bool) =>
      <div className="type-selector-choices">
        <h3> {ReasonReact.string("Definitions")} </h3>
        {ReasonReact.array(
           Belt.Array.map(
             Belt.Map.String.toArray(
               Belt.Map.String.keep(
                 definitions,
                 (_definitionID: definitionID, definition: definition) =>
                 filterFunction(definition)
                 && canConnectToNib(definition, nib.isSource)
               ),
             ),
             ((definitionID: definitionID, definition: definition)) =>
             <a
               key=definitionID
               className={
                 self.state.definitionID == Some(definitionID) ?
                   "selected" : ""
               }
               onClick={_event => self.send(SelectDefinition(definitionID))}>
               {ReasonReact.string(getDisplayName(definition, "en"))}
             </a>
           ),
         )}
      </div>;

    let nodeTypeLink = (kind, name) =>
      <a
        className={self.state.definedNodeKind == Some(kind) ? "selected" : ""}
        onClick={_event => self.send(SetDefinedNodeKind(kind))}>
        {ReasonReact.string(name)}
      </a>;

    <>
      <h2> {ReasonReact.string("Create a node")} </h2>
      <div className="type-selector-menu">
        <div className="type-selector-categories">
          <h3> {ReasonReact.string("Category")} </h3>
          {renderCategory("Defined", AllCategory)}
          {nib.isSource ?
             ReasonReact.null :
             <>
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
        {switch (self.state.category) {
         | None => ReasonReact.null
         | Some(category) =>
           switch (category) {
           | NumberCategory => nodeSelector(isNumberConstant)
           | AllCategory => nodeSelector(_ => true)
           }
         }}
        {switch (self.state.definitionID) {
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
                    {nib.isSource ?
                       ReasonReact.null :
                       <a onClick={_event => self.send(AddValue)}>
                         {ReasonReact.string("value")}
                       </a>}
                  </>
                | InterfaceImplementation(_) =>
                  <>
                    {nodeTypeLink(
                       FunctionPointerCallNode,
                       "function pointer call",
                     )}
                    {nib.isSource ?
                       ReasonReact.null :
                       <a
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
                  nib.isSource ?
                    <>
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
                    </> :
                    <>
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

                | _ => <> {ReasonReact.string("TODO")} </>
                }}
             </div>
             {switch (self.state.definedNodeKind) {
              | None => ReasonReact.null
              | Some(definedNodeKind) =>
                let display =
                  displayNode(
                    {
                      scope: getScope(nib, nodes),
                      kind:
                        DefinedNode({kind: definedNodeKind, definitionID}),
                    },
                    definitions,
                    "en",
                  );

                <div>
                  <h3>
                    {ReasonReact.string(nib.isSource ? "Input" : "Output")}
                  </h3>
                  {ReasonReact.array(
                     Belt.List.toArray(
                       Belt.List.map(
                         nib.isSource ? display.inputs : display.outputs,
                         (displayNib: displayNib) =>
                         <a
                           key={nibKey(displayNib.nib)}
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
                           {ReasonReact.string(
                              maybeNameless(displayNib.name),
                            )}
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
  },
};
