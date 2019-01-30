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
  | NumberCategory;

type state = {category: option(category)};

type action =
  | SelectCategory(category);

let isNumberConstant = (definition: definition): bool =>
  switch (definition.implementation) {
  | ConstantImplementation(NumberValue(_)) => true
  | _ => false
  };

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
  initialState: () => {category: None},
  reducer: (action: action, _state: state) =>
    switch (action) {
    | SelectCategory(category) =>
      ReasonReact.Update({category: Some(category)})
    },
  render: self => {
    let scope =
      switch (nib.connectionSide.node) {
      | GraphConnection => GraphScope
      | NodeConnection(nodeID) =>
        let node = Belt.Map.String.getExn(nodes, nodeID);
        isFunctionDefinitionNode(node) ? NodeScope(nodeID) : node.scope;
      };

    let renderCategory = (name: string, category: category) =>
      <a
        onClick={_event => self.send(SelectCategory(category))}
        className={self.state.category == Some(category) ? "selected" : ""}>
        {ReasonReact.string(name)}
      </a>;

    let nodeSelector = (name: string, filterFunction: definition => bool) =>
      <div className="type-selector-choices">
        <h3> {ReasonReact.string(name ++ "s")} </h3>
        {ReasonReact.array(
           Belt.Array.map(
             Belt.Map.String.toArray(
               Belt.Map.String.keep(
                 definitions,
                 (_definitionID: definitionID, definition: definition) =>
                 filterFunction(definition)
               ),
             ),
             ((definitionID: definitionID, definition: definition)) =>
             <a
               key=definitionID
               onClick={_event =>
                 emit(
                   AddNode({
                     node: {
                       kind: DefinedNode({kind: ValueNode, definitionID}),
                       scope,
                     },
                     explicitConnectionSide: nib,
                     connectionNib: ValueConnection,
                   }),
                 )
               }>
               {ReasonReact.string(getDisplayName(definition, "en"))}
             </a>
           ),
         )}
      </div>;

    <div className="type-selector-menu">
      <div className="type-selector-categories">
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
             {renderCategory("Number", NumberCategory)}
           </>}
      </div>
      {switch (self.state.category) {
       | None => ReasonReact.null
       | Some(category) =>
         switch (category) {
         | NumberCategory => nodeSelector("Number", isNumberConstant)
         }
       }}
    </div>;
  },
};
